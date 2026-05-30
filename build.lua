#!/usr/bin/env lua
-- os.execute("echo '[***BUILDING math_utils***]'")
-- os.execute("gcc math_utils.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
-- os.execute("echo '[***BUILDING main object file***]'")
-- os.execute(
-- 	"gcc -o main.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

-- os.execute("echo '[***BUILDING math_utils object file***]'")
-- os.execute(
--	"gcc -c math_utils.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")


os.execute("echo '[***BUILDING***]'")
os.execute(
	"gcc main.c -o game -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
