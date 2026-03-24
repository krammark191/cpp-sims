#include "simulators/chess/chess_scene.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#if defined(CPP_SIMULATORS_WINDOW) && defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>
#endif

#if defined(CPP_SIMULATORS_WINDOW) && defined(__linux__)
#include <GL/gl.h>
#endif

#if defined(CPP_SIMULATORS_WINDOW) && defined(_WIN32)
#include <GL/gl.h>
#endif

#include "simulators/chess/chess_legacy_bridge.h"
#include "simulators/common/scene_render_utils.h"

namespace
{
constexpr double kNativeBoardSize = SIZE_SQUARE * 10.0;
constexpr double kBoardMargin = 16.0;
constexpr double kPanelGap = 18.0;
constexpr double kPanelEdge = 18.0;
constexpr double kPanelSplit = 0.54;

char pieceLetter(const ChessPiece * piece)
{
   if (piece == nullptr)
      return ' ';

   char letter = '?';
   switch (piece->getType())
   {
      case KING:   letter = 'K'; break;
      case QUEEN:  letter = 'Q'; break;
      case ROOK:   letter = 'R'; break;
      case BISHOP: letter = 'B'; break;
      case KNIGHT: letter = 'N'; break;
      case PAWN:   letter = 'P'; break;
      case SPACE:
      default:     return ' ';
   }

   return piece->isWhite() ? letter : static_cast<char>(std::tolower(letter));
}

std::string sideName(bool white)
{
   return white ? "White" : "Black";
}

std::string squareName(const ChessPosition & square)
{
   std::string name = "a1";
   name[0] = static_cast<char>('a' + square.getCol());
   name[1] = static_cast<char>('1' + square.getRow());
   return name;
}

std::string promotionName(PieceType type)
{
   switch (type)
   {
      case QUEEN:  return "Queen";
      case ROOK:   return "Rook";
      case BISHOP: return "Bishop";
      case KNIGHT: return "Knight";
      default:     return "Queen";
   }
}

std::string formatClock(double totalSeconds)
{
   const int seconds = std::max(0, static_cast<int>(totalSeconds));
   const int minutesPart = seconds / 60;
   const int secondsPart = seconds % 60;

   std::ostringstream out;
   out << minutesPart << ':';
   if (secondsPart < 10)
      out << '0';
   out << secondsPart;
   return out.str();
}

std::string formatMoveLabel(const ChessMove & move)
{
   return squareName(move.getSource()) + " -> " + squareName(move.getDest());
}

#ifdef CPP_SIMULATORS_WINDOW
void drawPieceIcon(ChessOgstream & gout,
                   PieceType type,
                   bool whitePiece,
                   double centerX,
                   double centerY,
                   double scale)
{
   glPushMatrix();
   glTranslated(centerX - (SIZE_SQUARE * 1.5), centerY - (SIZE_SQUARE * 1.5), 0.0);
   glScaled(scale, scale, 1.0);

   const ChessPosition iconPosition(0, 0);
   switch (type)
   {
      case KING:   gout.drawKing(iconPosition, !whitePiece);   break;
      case QUEEN:  gout.drawQueen(iconPosition, !whitePiece);  break;
      case ROOK:   gout.drawRook(iconPosition, !whitePiece);   break;
      case BISHOP: gout.drawBishop(iconPosition, !whitePiece); break;
      case KNIGHT: gout.drawKnight(iconPosition, !whitePiece); break;
      case PAWN:   gout.drawPawn(iconPosition, !whitePiece);   break;
      case SPACE:
      default:     break;
   }

   glPopMatrix();
}

void drawCapturedPiecesGrid(ChessOgstream & gout,
                            const std::vector<PieceType> & captures,
                            bool whitePieces,
                            const Position & bottomLeft,
                            const Position & topRight)
{
   const double startX = bottomLeft.getX() + 26.0;
   const double startY = topRight.getY() - 82.0;
   const double stepX = 40.0;
   const double stepY = 42.0;
   const int columns = std::max(1, static_cast<int>((topRight.getX() - bottomLeft.getX() - 36.0) / stepX));

   for (std::size_t i = 0; i < captures.size(); ++i)
   {
      const int column = static_cast<int>(i % columns);
      const int row = static_cast<int>(i / columns);
      drawPieceIcon(gout,
                    captures[i],
                    whitePieces,
                    startX + column * stepX,
                    startY - row * stepY,
                    1.35);
   }
}
#endif
}

struct ChessScene::Impl
{
   bool active = false;
   double secondsSinceEnter = 0.0;
   InputState input;
   InputState previousInput;
   std::unique_ptr<ChessBoard> board;
   ChessPosition cursor = ChessPosition(4, 1);
   ChessPosition selected;
   std::vector<ChessMove> pendingPromotionMoves;
   std::size_t pendingPromotionIndex = 0;
   std::string status;
   std::string whiteLastMoveText;
   std::string blackLastMoveText;
   std::vector<PieceType> whiteCaptures;
   std::vector<PieceType> blackCaptures;
   double whiteElapsedSeconds = 0.0;
   double blackElapsedSeconds = 0.0;

   Impl()
   {
      selected.setInvalid();
      resetBoard();
   }

   void resetBoard()
   {
      board = std::make_unique<ChessBoard>(nullptr);
      cursor = ChessPosition(4, 1);
      selected.setInvalid();
      pendingPromotionMoves.clear();
      pendingPromotionIndex = 0;
      whiteLastMoveText.clear();
      blackLastMoveText.clear();
      whiteCaptures.clear();
      blackCaptures.clear();
      whiteElapsedSeconds = 0.0;
      blackElapsedSeconds = 0.0;
   }
};

ChessScene::ChessScene() :
   bindings(
   {
      {"Move cursor", "Arrow Keys", nullptr},
      {"Select square", "Space", nullptr},
      {"Promotion choice", "Left/Right", nullptr},
      {"Pause menu", "Esc", nullptr}
   }),
   impl(std::make_unique<Impl>())
{
}

ChessScene::~ChessScene() = default;

namespace
{
bool isPressed(bool current, bool previous)
{
   return current && !previous;
}

std::vector<ChessMove> legalMovesFrom(const ChessBoard & board, const ChessPosition & source)
{
   std::vector<ChessMove> moves;
   ChessPiece * piece = board.getPiece(source);
   if (piece == nullptr || piece->getType() == SPACE)
      return moves;

   std::set<ChessMove> pseudoLegal;
   piece->getMoves(pseudoLegal, board);
   for (const ChessMove & move : pseudoLegal)
      if (board.isMoveLegal(move))
         moves.push_back(move);

   return moves;
}

bool currentSideHasLegalMove(const ChessBoard & board)
{
   for (int col = 0; col < 8; ++col)
      for (int row = 0; row < 8; ++row)
      {
         ChessPosition square(col, row);
         ChessPiece * piece = board.getPiece(square);
         if (piece == nullptr || piece->getType() == SPACE || piece->isWhite() != board.whiteTurn())
            continue;

         std::set<ChessMove> pseudoLegal;
         piece->getMoves(pseudoLegal, board);
         for (const ChessMove & move : pseudoLegal)
            if (board.isMoveLegal(move))
               return true;
      }

   return false;
}

void refreshStatus(ChessScene::Impl & impl)
{
   if (!impl.pendingPromotionMoves.empty())
   {
      const PieceType promote = impl.pendingPromotionMoves[impl.pendingPromotionIndex].getPromote();
      impl.status =
         "Promotion pending. Left/Right choose, Space confirms " + promotionName(promote) + ".";
      return;
   }

   const bool whiteTurn = impl.board->whiteTurn();
   const bool inCheck = impl.board->isInCheck(whiteTurn);
   const bool hasLegalMove = currentSideHasLegalMove(*impl.board);

   if (!hasLegalMove && inCheck)
      impl.status = sideName(whiteTurn) + " is checkmated.";
   else if (!hasLegalMove)
      impl.status = "Stalemate.";
   else if (inCheck)
      impl.status = sideName(whiteTurn) + " to move. Check.";
   else
      impl.status = sideName(whiteTurn) + " to move.";

   if (impl.selected.isValid())
      impl.status += " Selected " + squareName(impl.selected) + ".";
   else
      impl.status += " Cursor " + squareName(impl.cursor) + ".";

}

void moveCursor(ChessScene::Impl & impl, int dCol, int dRow)
{
   int col = impl.cursor.getCol() + dCol;
   int row = impl.cursor.getRow() + dRow;
   col = std::max(0, std::min(7, col));
   row = std::max(0, std::min(7, row));
   impl.cursor = ChessPosition(col, row);
}

void executeMove(ChessScene::Impl & impl, const ChessMove & move)
{
   const bool whiteMover = impl.board->whiteTurn();
   if (move.getCapture() != SPACE)
   {
      if (whiteMover)
         impl.whiteCaptures.push_back(move.getCapture());
      else
         impl.blackCaptures.push_back(move.getCapture());
   }

   impl.board->move(move);
   if (whiteMover)
      impl.whiteLastMoveText = formatMoveLabel(move);
   else
      impl.blackLastMoveText = formatMoveLabel(move);
   impl.selected.setInvalid();
   impl.pendingPromotionMoves.clear();
   impl.pendingPromotionIndex = 0;
   refreshStatus(impl);
}

void handleSelection(ChessScene::Impl & impl)
{
   if (!impl.pendingPromotionMoves.empty())
   {
      executeMove(impl, impl.pendingPromotionMoves[impl.pendingPromotionIndex]);
      return;
   }

   ChessPiece * currentPiece = impl.board->getPiece(impl.cursor);
   const bool whiteTurn = impl.board->whiteTurn();

   if (!impl.selected.isValid())
   {
      if (currentPiece != nullptr &&
          currentPiece->getType() != SPACE &&
          currentPiece->isWhite() == whiteTurn)
      {
         impl.selected = impl.cursor;
      }
      refreshStatus(impl);
      return;
   }

   if (impl.cursor == impl.selected)
   {
      impl.selected.setInvalid();
      refreshStatus(impl);
      return;
   }

   const auto moves = legalMovesFrom(*impl.board, impl.selected);
   std::vector<ChessMove> matches;
   for (const ChessMove & move : moves)
      if (move.getDest() == impl.cursor)
         matches.push_back(move);

   if (matches.empty())
   {
      if (currentPiece != nullptr &&
          currentPiece->getType() != SPACE &&
          currentPiece->isWhite() == whiteTurn)
         impl.selected = impl.cursor;
      else
         impl.selected.setInvalid();

      refreshStatus(impl);
      return;
   }

   if (matches.size() == 1)
   {
      executeMove(impl, matches.front());
      return;
   }

   impl.pendingPromotionMoves = matches;
   impl.pendingPromotionIndex = 0;
   refreshStatus(impl);
}
}

SimulatorId ChessScene::id() const
{
   return SimulatorId::Chess;
}

const char * ChessScene::sceneName() const
{
   return "ChessScene";
}

const char * ChessScene::statusText() const
{
   return impl->status.c_str();
}

const std::vector<ControlBinding> & ChessScene::keybindings() const
{
   return bindings;
}

bool ChessScene::supportsRecordedPreview() const
{
   return true;
}

bool ChessScene::supportsFutureAiModes() const
{
   return true;
}

bool ChessScene::isReady() const
{
   return true;
}

bool ChessScene::isPlaceholder() const
{
   return false;
}

void ChessScene::enter()
{
   impl->active = true;
   refreshStatus(*impl);
}

void ChessScene::exit()
{
   impl->active = false;
}

void ChessScene::reset()
{
   impl->secondsSinceEnter = 0.0;
   impl->input = InputState{};
   impl->previousInput = InputState{};
   impl->resetBoard();
   refreshStatus(*impl);
}

void ChessScene::applyInput(const InputState & input)
{
   impl->input = input;
}

void ChessScene::update(double dtSeconds)
{
   if (!impl->active)
      return;

   impl->secondsSinceEnter += dtSeconds;
   if (impl->board->whiteTurn())
      impl->whiteElapsedSeconds += dtSeconds;
   else
      impl->blackElapsedSeconds += dtSeconds;

   const bool leftPressed = isPressed(impl->input.rotateLeft, impl->previousInput.rotateLeft);
   const bool rightPressed = isPressed(impl->input.rotateRight, impl->previousInput.rotateRight);
   const bool upPressed = isPressed(impl->input.upPressed, impl->previousInput.upPressed);
   const bool downPressed = isPressed(impl->input.downPressed, impl->previousInput.downPressed);
   const bool selectPressed =
      isPressed(impl->input.firePressed || impl->input.selectPressed,
                impl->previousInput.firePressed || impl->previousInput.selectPressed);

   if (!impl->pendingPromotionMoves.empty())
   {
      if (leftPressed || upPressed)
      {
         if (impl->pendingPromotionIndex == 0)
            impl->pendingPromotionIndex = impl->pendingPromotionMoves.size() - 1;
         else
            --impl->pendingPromotionIndex;
      }
      if (rightPressed || downPressed)
         impl->pendingPromotionIndex =
            (impl->pendingPromotionIndex + 1) % impl->pendingPromotionMoves.size();
   }
   else
   {
      if (leftPressed)
         moveCursor(*impl, -1, 0);
      if (rightPressed)
         moveCursor(*impl, 1, 0);
      if (upPressed)
         moveCursor(*impl, 0, 1);
      if (downPressed)
         moveCursor(*impl, 0, -1);
   }

   if (selectPressed)
      handleSelection(*impl);
   else
      refreshStatus(*impl);

   impl->previousInput = impl->input;
}

double ChessScene::elapsedSeconds() const
{
   return impl->secondsSinceEnter;
}

bool ChessScene::hasPendingPromotion() const
{
   return !impl->pendingPromotionMoves.empty();
}

bool ChessScene::isGameOver() const
{
   return impl->pendingPromotionMoves.empty() && !currentSideHasLegalMove(*impl->board);
}

void ChessScene::render(ogstream & gout, const Position & viewport) const
{
   const double boardSize = viewport.getY() - kBoardMargin * 2.0;
   const double boardScale = boardSize / kNativeBoardSize;
   const double boardLeft = std::max(kPanelEdge, (viewport.getX() - boardSize) * 0.5);
   const double boardBottom = kBoardMargin;
   const double leftPanelRight = std::max(kPanelEdge + 120.0, boardLeft - kPanelGap);
   const double rightPanelLeft = std::min(viewport.getX() - kPanelEdge - 120.0,
                                          boardLeft + boardSize + kPanelGap);
   const double topY = viewport.getY() - kPanelEdge;
   const double bottomY = kPanelEdge;
   const double splitY = bottomY + (topY - bottomY) * kPanelSplit;

   ChessOgstream legacyGout;
   impl->board->setOutput(&legacyGout);
#ifdef CPP_SIMULATORS_WINDOW
   glPushMatrix();
   glTranslated(boardLeft, boardBottom, 0.0);
   glScaled(boardScale, boardScale, 1.0);
   impl->board->display(impl->cursor, impl->selected);
   glPopMatrix();
#else
   impl->board->display(impl->cursor, impl->selected);
#endif

   const bool whiteTurn = impl->board->whiteTurn();
   const bool whiteCheck = impl->board->isInCheck(true);
   const bool blackCheck = impl->board->isInCheck(false);

   std::vector<std::string> whiteLines =
   {
      whiteTurn ? "Turn: active" : "Turn: waiting",
      whiteCheck ? "King: in check" : "King: safe",
      "Clock: " + formatClock(impl->whiteElapsedSeconds),
      "Last move: " + (impl->whiteLastMoveText.empty() ? std::string("-") : impl->whiteLastMoveText)
   };
   if (whiteTurn && impl->selected.isValid())
      whiteLines.push_back("Selected: " + squareName(impl->selected));
   whiteLines.push_back("Captures: " + std::to_string(impl->whiteCaptures.size()));

   std::vector<std::string> blackLines =
   {
      !whiteTurn ? "Turn: active" : "Turn: waiting",
      blackCheck ? "King: in check" : "King: safe",
      "Clock: " + formatClock(impl->blackElapsedSeconds),
      "Last move: " + (impl->blackLastMoveText.empty() ? std::string("-") : impl->blackLastMoveText)
   };
   if (!whiteTurn && impl->selected.isValid())
      blackLines.push_back("Selected: " + squareName(impl->selected));
   blackLines.push_back("Captures: " + std::to_string(impl->blackCaptures.size()));

   const Position whiteTopLeft(kPanelEdge, splitY + kPanelGap);
   const Position whiteTopRight(leftPanelRight, topY);
   const Position blackTopLeft(rightPanelLeft, splitY + kPanelGap);
   const Position blackTopRight(viewport.getX() - kPanelEdge, topY);
   const Position whiteBottomLeft(kPanelEdge, bottomY);
   const Position whiteBottomRight(leftPanelRight, splitY);
   const Position blackBottomLeft(rightPanelLeft, bottomY);
   const Position blackBottomRight(viewport.getX() - kPanelEdge, splitY);

   SceneRenderUtils::drawInfoPanel(gout,
                                   whiteTopLeft,
                                   whiteTopRight,
                                   "White",
                                   whiteLines,
                                   0.12, 0.14, 0.18);
   SceneRenderUtils::drawInfoPanel(gout,
                                   whiteBottomLeft,
                                   whiteBottomRight,
                                   "White Captures",
                                   {},
                                   0.15, 0.15, 0.18);
   SceneRenderUtils::drawInfoPanel(gout,
                                   blackTopLeft,
                                   blackTopRight,
                                   "Black",
                                   blackLines,
                                   0.12, 0.14, 0.18);
   SceneRenderUtils::drawInfoPanel(gout,
                                   blackBottomLeft,
                                   blackBottomRight,
                                   "Black Captures",
                                   {},
                                   0.15, 0.15, 0.18);

#ifdef CPP_SIMULATORS_WINDOW
   const Position activeTopLeft = whiteTurn ? whiteTopLeft : blackTopLeft;
   const Position activeTopRight = whiteTurn ? whiteTopRight : blackTopRight;
   SceneRenderUtils::drawOutline(gout, activeTopLeft, activeTopRight, 0.45, 1.00, 0.20);
   SceneRenderUtils::drawOutline(gout,
                                 Position(activeTopLeft.getX() + 2.0, activeTopLeft.getY() + 2.0),
                                 Position(activeTopRight.getX() - 2.0, activeTopRight.getY() - 2.0),
                                 0.30, 0.85, 0.15);
   drawCapturedPiecesGrid(legacyGout,
                          impl->whiteCaptures,
                          false,
                          whiteBottomLeft,
                          whiteBottomRight);
   drawCapturedPiecesGrid(legacyGout,
                          impl->blackCaptures,
                          true,
                          blackBottomLeft,
                          blackBottomRight);
#endif

   if (!impl->pendingPromotionMoves.empty())
   {
      SceneRenderUtils::drawInfoPanel(
         gout,
         Position(boardLeft + boardSize * 0.5 - 120.0, boardBottom + boardSize * 0.5 - 64.0),
         Position(boardLeft + boardSize * 0.5 + 120.0, boardBottom + boardSize * 0.5 + 48.0),
         "Promotion",
         {
            "Choose: " +
               promotionName(impl->pendingPromotionMoves[impl->pendingPromotionIndex].getPromote()),
            "Use Left/Right, then Space."
         },
         0.16, 0.18, 0.22);
   }
}
