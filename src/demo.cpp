
//This is Paul Shen's modifly demo.cpp
//Done at  09/10/2020
//Thie is the project main file.ZZ

#include "../hdr/demo.h"

#include <iostream>

int main()
{
   bool verbose = true;
   Command cmd;
   ItemTree *iTree = NULL;
   //list the table first
   helpCmd();
   //loop until the user want to quit
   do {
      cmd = getCommand(verbose);
      processCommand(iTree, cmd, verbose);
   } while (cmd != Quit);
   return 0;
}

// obtain the next command from the user (silent mode if verbose is false)
Command getCommand(bool verbose)
{
   wchar_t ch; // user-entered command character
   Command cmd; // translated command (None if invalid command entered)
   if (verbose) {
       std::wcout << L"Enter a command (QHNDPFVIR: H is help)";
   }
   std::wcout <<L"> ";
   std::wcin >> ch;
   ch = toupper(ch);
   switch (ch) {
      case Quit: //quit case
      case Help: //user want to show the menu
      case Print: // user want to print the tree
      case New: // user want to create a new tree
      case Delete: // user want to delete a tree
      case Find: // user want to find a node of the tree
      case Verbose:
      case Insert://use want to insert a new node
      case Remove://user want to remove a node
            // valid command entered
            cmd = (Command)(ch);
      break;

      default:
            // anything else is invalid
            cmd = None;
   }
   return cmd;
}

// let the user specify whether they wish to work on a single quadrant or an entire subtree
Modifier getModifier(bool verbose)
{
   wchar_t ch; // user-entered command character
   Modifier cmd; // translated modifier (Invalid unless valid choise made)
   if (verbose){ 
       std::wcout << L"Single quadrant (Q) or entire subtree (T)?";
   }
   std::wcout << L"> ";
   std::wcin >> ch;
   ch = toupper(ch);
   switch (ch) {
      case Tree:
      case Quad:
         // valid command entered
         cmd = (Modifier)(ch);
         break;

      default:
         // anything else is invalid
         cmd = Invalid;
   }
   return cmd;
}

// let the user specify a quaternary address
std::wstring getAddress(bool verbose)
{
   if (verbose){ 
       std::wcout << L"Enter the quaternary address or * for global" << std::endl;
   }
   std::wstring addr =L"";
   std::wcin >> addr;
   std::wstring check=L"*";
   if (addr == check) {
       addr = L"";
   }
   if (!Address::isValidAddr(addr) && verbose) {
      std::wcout << L"WARNING: " << addr << L" is not a valid address" << std::endl;
      //we do not need next line since it is not in the requirment
      //std::cout << "(either just *, or 1-" << Address::MaxAddr << " digits 0-3)" << std::endl;
   }
   return addr;
}

void helpCmd()
{
   std::wcout << std::endl;
   std::wcout << L"Welcome to the ItemTree demo" << std::endl;
   std::wcout << "Enter any one of the following commands:" << std::endl;
   std::wcout << L"    Q to quit" << std::endl;
   std::wcout << L"    D to display this menu" << std::endl;
   std::wcout << L"    V to toggle verbose mode (prompts/messages)" << std::endl;
   std::wcout << L"    N to create a new tree (deletes old)" << std::endl;
   std::wcout << L"    D to delete the current tree" << std::endl;
   std::wcout << L"    P to print items from the tree" << std::endl;
   std::wcout << L"    I to insert new elements in the tree" << std::endl;
   std::wcout << L"    R to remove elements from the tree" << std::endl;
   std::wcout << L"    F to find elements in the tree" << std::endl;
   std::wcout << std::endl;
}

// display the invalid command message (silent mode if verbose is false)
void errorCmd(bool verbose)
{
   if (verbose) {
      std::wcout << L"Error: an invalid command was entered, please try again" << std::endl;
      std::wcout << L"       (H is the command for the help menu)" << std::endl;
   }
}

// obtain any required arguments then apply the command to the tree
// (silent mode if verbose is true)
void processCommand(ItemTree* &IT, Command cmd, bool &verbose)
{
   switch (cmd) {
      case Quit: quitCmd(IT, verbose);
         break;
      case Help: helpCmd();
         break;
      case Verbose: verbose = !verbose;
         break;
      case Print: printCmd(IT, verbose);
         break;
      case Insert: insertCmd(IT, verbose);
         break;
      case Remove: removeCmd(IT, verbose);
         break;
      case Find: findCmd(IT, verbose);
         break;
      case New: newCmd(IT, verbose);
         break;
      case Delete: deleteCmd(IT, verbose);
         break;
      default: errorCmd(verbose);
         break;
   }
}

// terminate the program (silent mode if verbose is false)
void quitCmd(ItemTree* &IT, bool verbose)
{
   if (verbose) {
      std::wcout << L"Deallocating item tree and shutting down" << std::endl;
   }
   deleteCmd(IT, verbose);
   std::wcout << std::endl;
}

// delete and nullify current tree
// (silent mode if verbose is true)
void deleteCmd(ItemTree* &IT, bool verbose)
{
   if (IT) {
      delete IT;
      IT = NULL;
   } else if (verbose) {
      std::wcout << L"Warning: no item tree present to delete" << std::endl;
   }
}

// create a new item tree (deletes and replaces old)
// (silent mode if verbose is true)
void newCmd(ItemTree* &IT, bool verbose)
{
   if (IT) {
       deleteCmd(IT, verbose);
   }
   IT = new ItemTree;
}

// obtain any required arguments then print the requested tree components
// (silent mode if verbose is true)
void printCmd(ItemTree* IT, bool verbose)
{
   if (!IT) {
      if (verbose) {
          std::wcout << L"WARNING: no tree has been created to print" << std::endl;
      }
        return;
   }

   // see if the user wishes to print a single quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout << L"Error: neither quadrant nor tree was selected, ";
         std::wcout << L"please try again" << std::endl;
      }
   } while (m == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   // print the appropriate component
   if (m == Tree) {
      if (verbose)
         std::wcout << std::endl << L"Printing tree from \"" << addr << L"\"" << std::endl;
      IT->printTree(addr);
   } else if (m == Quad) {
      if (verbose)
         std::wcout << std::endl << L"Printing quad from \"" << addr << L"\"" << std::endl;
      IT->printQuad(addr);
   }
}

// prompt the user (if verbose) and read a line of text into the target
void getString(std::wstring prompt, std::wstring &target, bool verbose)
{
    if (verbose) {
        std::wcout << prompt;
    }
    std::getline(std::wcin, target);
}

// obtain any required arguments then inserted specified data into tree
// (silent mode if verbose is true)
void insertCmd(ItemTree* &IT, bool verbose)
{
    if (!IT) {
       std::wcout << L"WARNING: no tree exists to insert to" << std::endl;
       return;
    }
    //junk is not important just use to get rid of the line
    //title use for the tree title
    //desc is the description of the item
    //LL means the lower left corner of the tree
    //UR holds the upper right of the tree
    std::wstring junk, title, desc, LL, UR;
    std::getline(std::wcin, junk); // discard leftover newline from buffer
    std::wstring input =L"Enter a single line of text for the new item title: >";
    getString(input,title,verbose);
    input = L"Enter a single line of text for the new item description: >",
    getString(input,desc,verbose);
    if (verbose) {
        std::wcout << L"Next provide the address of the item\'s lower left corner"<<std::endl;
    }
    LL = getAddress(verbose);
    if (verbose) {
        std::wcout << L"Next provide the address of the item\'s upper right corner"<<std::endl;
    }
    UR = getAddress(verbose);
    Item *i = new Item(title, desc, LL, UR);
    if (!i) {
       std::wcout << L"WARNING: item creation failed" << std::endl;
    } else {
       std::wstring t, d, l, u, q;
       i->lookup(t, d, l, u, q); // lookup to get the quadrant
       if (!IT->insert(i)) {
          if (verbose) {
              std::wcout << L"WARNING: item insert failed" << std::endl;
          }
          delete i;
          i = NULL;
       } else if(verbose == true){
           std::wcout << L"Item inserted in quadrant " << q << std::endl;
       }
    }
 }

// obtain any required arguments then search the requested tree components,
//     returning a pointer to the relevant item
// (null if not found, silent mode if verbose is true)
Item* findCmd(ItemTree* IT, bool verbose)
{
   if (!IT) {
      if (verbose) {
          std::wcout << L"WARNING: no tree has been created to search" << std::endl;
      }
      return NULL;
   }
   // see if the user wishes to print a search quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout <<L"Error: neither quadrant nor tree was selected, ";    
         std::wcout << L"please try again" << std::endl;
      }
   } while (m == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);
     //junk is not important just use to get rid of the line
    //title use for the tree title
    //desc is the description of the item
    //LL means the lower left corner of the tree
    //UR holds the upper right of the tree
   std::wstring junk, title, desc, LL, UR, quad;
   std::getline(std::wcin,junk); // discard leftover newline from buffer
    std::wstring input =L"Enter a single line of text for the title you are searching for: >";
   getString(input,title,verbose);
   Item* iptr = NULL;
   if (m == Quad) {
       iptr = IT->findInQuad(title, addr);
   }
   else {
       iptr = IT->findInTree(title, addr);
   }

   if (!iptr) {
      std::wcout << L"No matching item found for \"" << title << "\" in ";
      std::wcout << addr << std::endl;
   } else {
      std::wcout <<L"Found: ";
      iptr->print();
   }
   return iptr;
}

// obtain any required arguments then remove the requested tree components
// (silent mode if verbose is true)
void removeCmd(ItemTree* &IT, bool verbose)
{
   if (!IT) {
      if (verbose) {
          std::wcout <<L"WARNING: no tree has been created yet" << std::endl;
      }
      return;
   }
   // see if the user wishes to print a search quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout <<L"Error: neither quadrant nor tree was selected, ";
         std::wcout <<L"please try again" << std::endl;
      }
   } while (m == Invalid);

   std::wstring junk, title, desc, LL, UR, quad;
   std::getline(std::wcin, junk); // discard leftover newline from buffer
    std::wstring input = L"Enter a single line of text for the title of the item you wish removed >";
   getString(input,title, verbose);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   Item* iptr = NULL;
   if (m == Quad){ 
       iptr = IT->findInQuad(title, addr);
   }
   else iptr = IT->findInTree(title, addr);

   if (!iptr) {
      std::wcout << L"No matching item found for \"" << title << L"\" in ";
      std::wcout << addr << std::endl;
   } else {
       // ************ TRY THE REMOVE HERE ****************
       if (IT->remove(iptr)) {
           std::wcout << L"Removed item \"" << title << L"\"" << std::endl;
       } else {
           std::wcout << L"Remove unsuccessful for item \"" << title << L"\"" << std::endl;
       }
   }
}

