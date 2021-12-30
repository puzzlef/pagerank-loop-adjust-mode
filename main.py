# https://www.kaggle.com/wolfram77/puzzlef-pagerank-adjust-mode
import os
from IPython.display import FileLink
src="pagerank-adjust-mode"
inp="/kaggle/input/graphs"
out="{}.txt".format(src)
!printf "" > "$out"
display(FileLink(out))
!echo ""

# Download program
!rm -rf $src
!git clone https://github.com/puzzlef/$src
!echo ""

# Run
!g++ -std=c++17 -O3 $src/main.cxx
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/CollegeMsg.txt             2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/email-Eu-core-temporal.txt 2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/sx-mathoverflow.txt        2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/sx-askubuntu.txt           2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/sx-superuser.txt           2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/wiki-talk-temporal.txt     2>&1 | tee -a "$out"
!ulimit -s unlimited && stdbuf --output=L ./a.out $inp/sx-stackoverflow.txt       2>&1 | tee -a "$out"
