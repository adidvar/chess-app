import os
import subprocess

sourse = []

compiler = "g++"

include = ["-Iinclude" , "-ID:\Developing\SFML32\include"]
libpaths = ["-LD:\Developing\SFML32\lib"]
libs = [ "-lsfml-graphics-s" , "-lfreetype" , "-lsfml-window-s" , "-lopengl32" , "-lgdi32" , "-lsfml-system-s" , "-lwinmm" ]
defines = [ "-DSFML_STATIC" ]
flags = ["-O3" , "-m32" , "-mfpmath=sse" , "-Ofast" , "-flto" , "-march=native" , "-static-libgcc" , "-static-libstdc++"];

def walk(dir):
  for name in os.listdir(dir):
    path = os.path.join(dir, name)
    if os.path.isfile(path):
        if(path.split(".")[1] == "cpp"):
          sourse.append(path)
    else:
        walk(path)
walk(os.getcwd() + "\sourse")


subprocess.run([compiler] + sourse + include + libpaths + libs + defines + flags)

