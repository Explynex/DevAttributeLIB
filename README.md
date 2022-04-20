═══════════════════════════════╣ INFO ╠════════════════════════════════                            
 The library is intended to simplify development in console applications. 

 Installation information:
 
	1. Download devattribute.h and devattribute.lib
	2. Move these two files to your project.
	3. Put #include "devattribute.h".
	DONE!
	
════════════════════════════╣ HOW TO USE ╠══════════════════════════════                            
Functionality will expand over time.
 1. Switch std::string. Construction ↓↓↓
 
	![SWITCH_STRING](https://user-images.githubusercontent.com/96521396/163830521-805e01e8-cbe6-4787-9227-ac8583d3c0bb.png)

 2. SetColor function. Preview: [setColor](https://user-images.githubusercontent.com/96521396/163830904-7131311e-2411-4bb9-bf29-43bccfaab941.jpg)                  
    The function accepts color names from enum consoleColors as arguments:
    
     ![enu](https://user-images.githubusercontent.com/96521396/163831535-166f6bea-a60b-4197-b4ef-1244b426ac74.png)
 3. GotoXY function.                                                              
    The function takes x,y position as arguments and moves the cursor to the specified coordinates.
    It is also possible to pass text for output and paint it in some color from enum. The last 2 arguments are optional.               
    
     ![GotoXY](https://user-images.githubusercontent.com/96521396/163831752-6e471c61-ca2f-4071-b7c6-781623a2ab75.jpg)
 4. Сleaning function.                                      
    May be useful after GotoXY. Clears the specified area with spaces. Takes an x,y position, width and height.               
    
     ![cleaning](https://user-images.githubusercontent.com/96521396/163831029-5ccf081f-36b6-4ba1-8380-ad835f223d31.jpg)

 5. Sheet generator.                                    
    Generates a table in the console. Takes x,y position, number of columns, number of rows, cell length and cell height.
    
    ![SheetGenerator](https://user-images.githubusercontent.com/96521396/163832976-d90281d3-3901-4e14-bf78-71cbaac6083d.jpg)
 6. GetPath function.                         
    Returns the path where the project is located or the name of the current user. Modes: "path" or "username".   
    
    ![getPath](https://user-images.githubusercontent.com/96521396/163834280-6651bd44-d6fd-4d2a-b061-37457aba5131.png)
 7. FileNameList function.                  
    Returns a vector of names of all files found in the folder at the specified path.
    It accepts parameters: only_names - if 'n' returns the full path to the file, path_to_dir - the path to the folder,
    if "auto" returns the contents of the current project folder.   
    
    ![filenamelist](https://user-images.githubusercontent.com/96521396/163835296-f1331d5f-c897-4eb3-87c8-a0bb32b3f288.png)
  8. PrintFilter function.                            
     Allows you to monitor user input in real time. Works with EN-UA-RU localizations.                                                
     Parameters: 
       - x,y position, valid string length, 
       - mode: if "str" - all available characters are allowed, if "calc" - digits, dot and minus, if "digit" - only digits.
       - min,max - the range of allowed characters, example: "A","z",    
       - minDig,maxDig - for numbers, forbids entering a number greater or less than the specified one.                
                                                                                                                                                                   
     Attention! Returns std::string. If using for numbers, initialize like so: 
     
                 int n = std::stoi(printFilter(1,1,2,"digit")) - to int,
                 std::stof() - to float, std::stod() - to double, std::stoull - to DWORD.      
     If you press ESC while typing, the function will return 0. This can be used to implement an exit!       
     
   ![PrintFilter](https://user-images.githubusercontent.com/96521396/163837096-7889ca2e-ff21-43d9-9ab2-4c4ce27b6479.jpg)
