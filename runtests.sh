#! /bin/bash

explore(){
  local dir=$1
  local input=$2
  for file in "$dir"/*;do
      if [ -f $file ] ; then
         ./$input < $file          
      fi
      if [ -d $file ]; then
        explore "$file" "$input"
      fi
  done
}


read -p "please enter a directory name " dir
echo "The direcotry name you enter is $dir "
read -p "please enter an excutable name " exc
echo "The excutable nme you enter is $exc "
echo "Now testing these two are valid or not "

#test directory is readable or not
if [ -d "$dir" ] && [ -r "$dir" ]; then
   echo " $dir is a good directory "
   #test if it is excuteable or not, the -f part can be skipped
   if [ -f $exc ] && [ -x $exc ] ; then
     echo " $exc is a good excutable"
     echo " now start the runtest"
     explore "$dir" "$exc"
   else
      echo " $exc is not a good excutable"
   fi
else
   echo " $dir is not a good directory"
fi
