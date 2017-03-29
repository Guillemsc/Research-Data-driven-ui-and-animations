# Research: Data driven ui and animations
## 1. Data driven UI
### 1.1 Introduction
As we have less things on our projects being configured from code, the better. That applies to console configurations, player starting stats, positions, animations, etc, and also, to UI management and creation. That's when data driven UI begins.
This part of the research is going to talk about the creation and implementation of a UI data driven manager on our code, that will interpretate XML commands, to create full UI systems.

### 1.2 Initial thoughts
To begin with, we have to determinate the type of UI base system that we are going to use, because that is going to change completly the internal organization of the data manager. Since it's impossibe to create a general data manager without knowing the type of UI that is going to use, this one is going to be based on the UI that i've created on the past. The general structure of this UI is the following:

![UI structure](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/UI.png?raw=true)

The way in which this works, is by creating a Window, and from there, we will create all the different UI elements that will be childs of that window.

Once we have defined our UI system, we need to know the way that our UI data driven manager is going to work. It should have the following abilities:
- Different UI for different scenes.
- Dinamic load and unload of UI when changing of scene.
- Basic UI functions.
- Ability to have relations between different UI elements.
- Easy XML creation implementation/creation.

From that, we have to know which XML commands will the code understand, for examle:
```C++
<Element type = "Image" name ="image" position_x ="700" position_y ="200" rect_x ="0" rect_y =" 0" .../>

<Element type ="Text" name ="text" position_x ="775" position_y = "368" .../>
```
### 1.3 UI Elements implementation
First of all, since we have to have different scene recongition, we will start with scene definition:
```C++
 <scene name = "menu_scene"> // Name of the scene should match with the name set on code.
  </scene>
```
Everything that we will create in a scene should be placed inside the scene brackets.
Next, we need the windows:
```C++
<Window name = "main_window" position_x = "700" position_y = "200" size_w = "225" size_h = "219" blit_layer = "1" dinamic = "false" is_ui ="true">
</Window>
```
From that, evey UI elements that pertains to this window should be created inside the window brackets.
Then we need the actual elements. They also have name to then be able to refeer to them in some actions.
We can add the interpretation of any the UI elements that we have on the UI, for example: 
```C++
 <Element type ="Text" name ="text" position_x ="754" position_y = "265" font ="2" spacing ="0" color_r ="168" color_g ="134" color_b ="0">
```
As we can see, we are creating a text here, but where do we define the actual text that is goint to print? We need to be able to recieve other parameters!
```C++
<Element type ="Text" name ="text" position_x ="754" position_y = "265" font ="2" spacing ="0" color_r ="168" color_g ="134" color_b ="0">
     <var text = "Start Game"/>
     <var enabled ="false"/> // In this case the element won't start enabled
</Element>
```
With that, we can create any type of element, and in the case that we detect that the scene is changed, we unload and we load the new UI.
![Code structure](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Start.png?raw=true)

### 1.4 Variables
As it can be very usefull when creating data driven ui actions, it can be interesting the creation of data driven dynamic variables to expand the uses that we can give to our UI. This is not mandatory, but can be usefull:
```C++
<new_var start_value ="0" name ="percentage" max_value ="100" min_value ="0"/>
```
### 1.5 Actions
Here is when the thing starts to become a little bit messy. It's important that we can define basic UI behaviour to really explore what data driven UI can do. 
From this point, the possibilities of actions and interactions are infinite, to the level of even represent full c++ code, so let's try not go to deep on it.
Actions also should go inside the scene bracket.
```C++
<Scene>
   <Action>
   </Action>
</Scene>
```
Next, thanks to the ability to reference to other UI elements thanks to it's names, we can start to build basic behaviours, for examle:
An easy one:
```C++
<act name ="button_start" if_left_pressed ="true"> // If button left pressed
   <act load_scene ="second_scene"/> // Load scene
</act>
```
A more complex one:
```C++
<act name ="button_options" if_left_pressed ="true"> // If button left pressed
     <act name ="options_window" if_enabled ="true"> // If options window is enabled
          // Do an animation
           <act name = "options_window" animation = "movement" movement_type = "smooth" destination_x = "-500" destination_y = "130" time = "0.5">
                  // When animation is finished, disable element
                  <act name ="options_window" set_enabled ="false"/>
           </act>
     </act>
     
     <act name ="options_window" if_enabled ="false"> // If options window is disabled
           <act name ="options_window" set_enabled ="1"/> // Enable it
           // Do an animation
           <act name = "options_window" animation = "movement" movement_type = "smooth" destination_x = "400" destination_y = "130" time = "0.5"/>
      </act>
</act>
 ```
