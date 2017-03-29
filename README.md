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
Then we need the actual elements
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
