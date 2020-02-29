BIN=${GBDKDIR}/bin
OBJ=./obj
OPTIONS=--Werror --opt-code-size --max-allocs-per-node 30000

build:
	mkdir -p $(OBJ)
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/score.c       -o $(OBJ)/score.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/game.c        -o $(OBJ)/game.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/control.c     -o $(OBJ)/control.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/ball.c        -o $(OBJ)/ball.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/text.c        -o $(OBJ)/text.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/missile.c     -o $(OBJ)/missile.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/paddle.c      -o $(OBJ)/paddle.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/background.c  -o $(OBJ)/background.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/collision.c   -o $(OBJ)/collision.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/link.c        -o $(OBJ)/link.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/graphics.c    -o $(OBJ)/graphics.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/sound.c       -o $(OBJ)/sound.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/titlescreen.c -o $(OBJ)/titlescreen.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/bonus.c       -o $(OBJ)/bonus.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/paddle.c      -o $(OBJ)/paddle.rel
	$(BIN)/gbdk-n-compile.sh $(OPTIONS) src/main.c        -o $(OBJ)/main.rel
	$(BIN)/gbdk-n-link.sh $(OBJ)/*.rel -o $(OBJ)/game.ihx
	$(BIN)/gbdk-n-make-rom.sh $(OBJ)/game.ihx game.gb

clean:
	rm -rf $(OBJ)
	rm -f game.gb