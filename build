#!/bin/bash

FLAGS="`pkg-config --cflags --libs opengl glu glut` -lm"
SOURCES="`find src/ -name *.c`"

# make sure out/ directory exists
mkdir -p out/

# for each source file compile as object file
for src in `echo $SOURCES`; do
  obj=out/${src#"src/"}.o
  # if object file exists
  if [ -f $obj ]; then
    obj_time=`stat $obj -c %Y`
    src_time=`stat $src -c %Y`
    # source file more recent. Then it was modified, so compile it again
    if [ $src_time -gt $obj_time ]; then
      gcc -c $FLAGS $src -o $obj
      echo "$src Recompiled"
    else
      echo "$obj is up-to-date"
    fi
  else # otherwise
    gcc -c $FLAGS $src -o $obj
    echo "$src Compiled"
  fi
done

gcc $FLAGS `find out/ -name *.o` -o flattybird.out
