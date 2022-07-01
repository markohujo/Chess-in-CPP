CXX = g++
CXX_FLAGS = -g -Wall -pedantic -Wextra -Werror -std=c++17

ifeq ($(pretty),0)
	CXX_FLAGS += -DNOTPRETTY
endif

LINK = -lstdc++fs
DIR = build
DOC = doc
BIN = hujomark

.PHONY: all
all: compile doc
	@echo "Finished compiling and generating documentation."

.PHONY: compile
compile: $(BIN)
	@echo "Finished compiling."

.PHONY: run
run: compile
	./$(BIN)

$(BIN): $(DIR)/Application.o $(DIR)/Board.o $(DIR)/Command.o $(DIR)/CommandFactory.o $(DIR)/Game.o $(DIR)/Interface.o $(DIR)/King.o $(DIR)/Move.o $(DIR)/Piece.o $(DIR)/Player.o $(DIR)/PlayerBot.o $(DIR)/PlayerHuman.o $(DIR)/Queen.o $(DIR)/Pawn.o $(DIR)/Rook.o $(DIR)/Knight.o $(DIR)/Bishop.o $(DIR)/Color.o $(DIR)/PieceType.o $(DIR)/UserInputException.o $(DIR)/SaveFileException.o $(DIR)/PlayerEasyBot.o $(DIR)/PlayerMediumBot.o $(DIR)/PlayerHardBot.o $(DIR)/GameMode.o $(DIR)/main.o
	$(CXX) $^ -o $@ $(LINK)

$(DIR)/%.o: src/%.cpp
	mkdir -p $(DIR)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

$(DIR)/%.o: src/*/%.cpp
	mkdir -p $(DIR)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

doc: src/*.h src/pieces/*.h src/players/*.h src/enums/*.h src/exceptions/*.h README.md
	doxygen Doxyfile
	@echo "Documentation generated."

# Used for generating zip folder that is going to be submitted to progtest
zip: Makefile Doxyfile README.md zadani.txt prohlaseni.txt examples/* src/* src/pieces/* src/players/* src/enums/* src/exceptions/*
	mkdir -p .archive/hujomark/
	cp -r Makefile Doxyfile README.md zadani.txt prohlaseni.txt examples/ src/ .archive/hujomark
	cd .archive/; zip -r ../hujomark.zip hujomark/
	rm -r .archive/

.PHONY: clean
clean:
	rm -rf $(BIN) $(DIR)/ $(DOC)/ hujomark.zip 2>/dev/null

.PHONY: cleansave
cleansave: clean
	rm -rf saves/

$(DIR)/Application.o: src/Application.cpp src/Application.h src/Interface.h src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/enums/GameMode.h src/Command.h src/enums/GameState.h src/CommandFactory.h src/exceptions/SaveFileException.h
$(DIR)/Board.o: src/Board.cpp src/Board.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h src/pieces/Pawn.h
$(DIR)/Command.o: src/Command.cpp src/Command.h src/Interface.h src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/enums/GameMode.h src/enums/GameState.h
$(DIR)/CommandFactory.o: src/CommandFactory.cpp src/CommandFactory.h src/Command.h src/Interface.h src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/enums/GameMode.h src/enums/GameState.h src/exceptions/SaveFileException.h
$(DIR)/Game.o: src/Game.cpp src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/enums/GameMode.h src/players/PlayerHuman.h src/players/PlayerBot.h src/pieces/Pawn.h src/pieces/Rook.h src/pieces/Knight.h src/pieces/Bishop.h src/pieces/Queen.h src/pieces/King.h src/exceptions/SaveFileException.h src/players/PlayerEasyBot.h src/players/PlayerMediumBot.h src/players/PlayerHardBot.h
$(DIR)/Interface.o: src/Interface.cpp src/Interface.h src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/enums/GameMode.h src/exceptions/UserInputException.h
$(DIR)/main.o: src/main.cpp src/Application.h src/Interface.h src/Game.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/enums/GameMode.h src/Command.h src/enums/GameState.h src/exceptions/UserInputException.h
$(DIR)/Move.o: src/Move.cpp src/Move.h src/Board.h src/pieces/Piece.h src/pieces/../enums/Color.h src/enums/PieceType.h src/pieces/Queen.h src/pieces/Pawn.h
$(DIR)/Color.o: src/enums/Color.cpp src/enums/Color.h
$(DIR)/GameMode.o: src/enums/GameMode.cpp src/enums/GameMode.h
$(DIR)/PieceType.o: src/enums/PieceType.cpp src/enums/PieceType.h
$(DIR)/SaveFileException.o: src/exceptions/SaveFileException.cpp src/exceptions/SaveFileException.h
$(DIR)/UserInputException.o: src/exceptions/UserInputException.cpp src/exceptions/UserInputException.h
$(DIR)/Bishop.o: src/pieces/Bishop.cpp src/pieces/Bishop.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/King.o: src/pieces/King.cpp src/pieces/King.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/Knight.o: src/pieces/Knight.cpp src/pieces/Knight.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/Pawn.o: src/pieces/Pawn.cpp src/pieces/Pawn.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/Piece.o: src/pieces/Piece.cpp src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/Queen.o: src/pieces/Queen.cpp src/pieces/Queen.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h src/pieces/Rook.h src/pieces/Bishop.h
$(DIR)/Rook.o: src/pieces/Rook.cpp src/pieces/Rook.h src/pieces/Piece.h src/pieces/../Move.h src/pieces/../Board.h src/pieces/../enums/PieceType.h src/pieces/../enums/Color.h
$(DIR)/PlayerBot.o: src/players/PlayerBot.cpp src/players/PlayerBot.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
$(DIR)/Player.o: src/players/Player.cpp src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
$(DIR)/PlayerEasyBot.o: src/players/PlayerEasyBot.cpp src/players/PlayerEasyBot.h src/players/PlayerBot.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
$(DIR)/PlayerHardBot.o: src/players/PlayerHardBot.cpp src/players/PlayerHardBot.h src/players/PlayerBot.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
$(DIR)/PlayerHuman.o: src/players/PlayerHuman.cpp src/players/PlayerHuman.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
$(DIR)/PlayerMediumBot.o: src/players/PlayerMediumBot.cpp src/players/PlayerMediumBot.h src/players/PlayerBot.h src/players/Player.h src/players/../pieces/Piece.h src/players/../pieces/../Move.h src/players/../pieces/../Board.h src/players/../pieces/../enums/PieceType.h src/players/../pieces/../enums/Color.h src/players/../Interface.h src/players/../Game.h src/players/../enums/GameMode.h
