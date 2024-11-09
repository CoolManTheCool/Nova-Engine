# Do not run this script
#
# TODO:
#
#
#
#
#

# 8/11/24 - Made this file so I can write comments.
# 18:14, Just finished build systems. Ready to do publish changes.
# 
# 8/25/24 - I did some ImGui work and finished the Gui Tree
# 3:40
# Its "pretty simple", 
#
# Create the bindings for the GUI,
# create a variable of abstract type to be shared throughout the program.
#
#GUI_System.setBinding("Window Debug Closed", false);
#GUI_System.setBinding("Camera Speed", 0);
#
# Define the lanbda that will be used
# to draw the GUI, use bindings to save and restore values.
#
#GUI_System.registerWindow([]() {
#   ImGui::Begin("Debug Window", std::any_cast<bool>(GUI_System.getBinding("Window Debug Closed")));
#   ImGui::Text("Test Text");
#   ImGui::SliderInt("Test Slider", std::any_cast<int>(GUI_System.getBinding("Camera Speed")), 0, 10);
#   ImGui::Text(("Test Slider: " + std::to_string(std::any_cast<int>(*GUI_System.getBinding("Camera Speed")))).c_str());
#   ImGui::End();
#});
# 
# 3:50 PM - Considering leaning more on Visual Studio Code to compile the project,
# This can have issues, like if you dont use visual studio code.
# Most of the issue is how we handle resources like shaders.
# I wish I could compile shaders on runtime,
# This would introduce overhead with starting the project
# Although It might make the project more cross platform
#
# 3:51 PM - Thinking about networking. 
# How do I want to go about making a server and a client?
# Do I want the server to be a seperate project, or just bundle it with the client?
# Bundling with the client would have the following pros:
# - Optional & Fast Singleplayer
# - Bigger Binaries
# 
# But Bundling with the client would have the following cons:
# - Debugging would be harder
# - Less Flexibility
# - Less Scalability
# 
# 3:57 PM - Just thought about how dangerous Dihydrogen Monoxide
# This chemical has a pH of 7, which is higher than all acids.
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
