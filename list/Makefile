FLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations\
	-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts\
	-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal\
	-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op\
	-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self\
	-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel\
	-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods\
	-Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand\
	-Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix\
	-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs\
	-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow\
	-flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie\
	-fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,$\
	float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,$\
	return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,$\
	vla-bound,vptr

INCL=-I./include/

OBJ_DIR=./objects/

SRC_DIR=./source/

GPP=g++ $(FLAGS) $(INCL)

OBJECTS=$(OBJ_DIR)main.o $(OBJ_DIR)list_funcs.o ../common/objects/errors.o\
	../common/objects/utilities.o ../stack/objects/stack_funcs.o ../stack/objects/verify_funcs.o

../list_run: $(OBJECTS)
	$(GPP) $(OBJECTS) -o ../list_run

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp
	$(GPP) -c $(SRC_DIR)main.cpp -o $(OBJ_DIR)main.o

$(OBJ_DIR)list_funcs.o: $(SRC_DIR)list_funcs.cpp
	$(GPP) -c $(SRC_DIR)list_funcs.cpp -o $(OBJ_DIR)list_funcs.o

clean:
	rm ./objects/*.o

dot_svg:
	dot ../dump.dot -Tsvg -o ../dump.svg

dot_png:
	dot ../dump.dot -Tpng -o ../dump.png
