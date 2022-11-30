#! /bin/bash 

echo "You totally pass $# arguments"

for arg in "$@";do
   if [ -f "$arg" ]; then
      if [ -r "$arg" ]; then
         echo "$arg is a readable file"
         git blame $arg|less
         head -n 5 $arg
         lineCount=$(wc -l < $arg)
         echo "there are total $lineCount lines in the file $arg"
         charCount=$(wc -m < $arg)
         echo "there are total $charCount characters in the file $arg"
      else
         echo "$arg is not a readable file"
      fi
   else
      echo "$arg is not a file"
   fi

done

