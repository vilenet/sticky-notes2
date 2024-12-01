# Sticky Notes

This is a minimally functional program that is already usable but not yet complete: additional settings need to be implemented, a database may need to be connected, code optimization is required, ...

### 1. Sticky Notes
![Sticky Notes](images/Sticky%20Notes.png)

### 2. Sticky Note Menu
![Sticky Note Menu Options](images/Sticky%20Note%20Menu.png)

### 3. Sticky Note Settings
![Sticky Note Settings Window](images/Sticky%20Note%20Menu%20Settings.png)

### 4. Settings (Title and Color Changed)

![Changed Title & Color](images/Sticky%20Note%20Menu%20Settings%20Changed%20Title%20Color.png)

This screenshot shows the settings window of the application, where the following changes have already been made:
- The note's title has been updated to "TODO".
- The note's color has been changed to green from the available palette.

These changes have been applied, demonstrating how users can customize each note to their preferences.


This is a Windows/Win32 dependent version. 

The cross-platform version will be available in the repository: https://github.com/vilenet/sticky-notes.git.


### Overview
This is the **second version** of the Sticky Notes program, designed to utilize a standard Win32 window with a built-in title bar. However, it introduces **custom behavior** for the window icon (located in the top-left corner), effectively transforming it into a functional button. This approach allows for creating a faux button using the window icon area, enhancing the window's interactivity without modifying the native title bar.

### Key Features
- Utilizes the standard **Win32 window** with a built-in title bar.
- Implements **custom functionality** for the window icon, turning it into a clickable button.
- Maximizes the **text note area** by avoiding additional UI elements, such as menu bars.
- Focuses on a **clean and distraction-free** interface, ensuring that the main area is dedicated to note-taking.

### Design Philosophy
The design goal of this version is to prioritize content visibility by maximizing the space available for notes. Unlike traditional applications, there is no menu bar below the Win32 title bar, as this would reduce the available space for note-taking. This minimalistic design choice provides a more immersive note-taking experience, focusing on simplicity and usability.

### Alternative Approach: Version with Custom Title Bar
In another version of the program, the standard **Win32 title bar is completely removed**, and a custom title bar is drawn manually using **FLTK**. This approach allows for:
- Full control over the window’s appearance and behavior.
- A more modern and customizable interface.
- Retaining a lightweight application footprint while providing a unique design.

### Usage
1. Run the program to open the Sticky Notes window.
2. Click on the window icon in the top-left corner to access additional features.
3. Enjoy a clutter-free space for writing and managing your notes.

### Future Enhancements
- Integration of additional features such as saving and loading notes.
- Option to switch between the standard Win32 title bar and the custom FLTK title bar.
- Potential support for color customization and theming.

### Technologies Used
- **C++**
- **Win32 API**
- **FLTK (Fast Light Toolkit)**

### License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

