# Research: Data driven ui and animations
## 1. Data driven UI
### 1.1 Introduction
As we have less things on our projects being configured from code, the better. That applies to console configurations, player starting stats, positions, animations, etc, and also, to UI management and creation. That's when data driven UI begins.
This part of the research is going to talk about the creation and implementation of a UI data driven manager on our code, that will interpretate XML commands, to create full UI systems.

### 1.2 Initial thoughts
To begin with, we have to determine the type of UI base system that we are going to use, because that is going to change completly the internal organization of the data manager. Since it's impossibe to create a general data manager without knowing the type of UI that is going to use, this one is going to be based on the UI that i've created on the past. The general structure of this UI is the following:

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
With that, evey UI elements that pertains to this window should be created inside the window brackets.
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
We can create any type of element, and in the case that we detect that the scene is changed, we unload and we load the new UI.
![Code structure](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Start.png?raw=true)

### 1.4 Variables
As it can be very usefull when creating data driven ui actions, it can be interesting the creation of data driven dynamic variables to expand the uses that we can give to our UI. This is not mandatory, but can be usefull:
```C++
<new_var start_value ="0" name ="percentage" max_value ="100" min_value ="0"/>
```
### 1.5 Actions
Here is when the thing starts to become a little bit messy. It's important that we can define basic UI behaviour to really explore what data driven UI can do. 
From this point, the possibilities of actions and interactions are infinite, to the level of even represent full c++ code, so let's try not get very complex.
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
<Action>
    <act name ="button_start" if_left_pressed ="true"> // If button left pressed
           <act load_scene ="second_scene"/> // Load scene
    </act>
</Action>
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
 With that we can archive a decent/easy UI XML creation.
 
![Code structure](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/check.png?raw=true)
 
 ## 2. UI Animations
 ### 2.1 Introduction
Apart from easy to use and functional, and UI has to be pleasant and visual to the user.
With that in mind, UI animations thake a hiuge part on the UI creation process and thoguht.
We can find a lot of examples on mobile games, where everyghing has to be soft, bouncy and pleasant to the use.

![UI image](https://farm4.staticflickr.com/3895/14819170192_829f0e9f5a_o.png)

 ### 2.2 Initial thoughts
To begin with, we have a huge amount of different animations that we could implement, but in this research we are just going to focus on UI movement animations, moving from one point to another, in a fixed amount of time.

![UI_Amazing](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Amazing.png?raw=true)

Going from there, we can have an element moving form point A to point B in a defined amount of time with a fixed speed. 
But having a constant speed is quite boring and not pleasant at all, so we can start looking at curves of movement, to have different effects, for example:
- Ease in
- Ease out
- Ease in out cubic
- Ease in elastic
- Ease out bounce
- Etc..

 ### 2.3 Bézier
We can archive this type of movements with Bézier curves, which are very used on software development:

![BezierLaXupa](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/CocoaDrawingGuide/Art/bezier_curve_2x.png)

You can find them, for example, in photoshop/illustrator/etc tools, to define paths, etc..
The Bézier curves are defined by this recursive function: 

![BezierLaXupa](https://wikimedia.org/api/rest_v1/media/math/render/svg/ed083d663a81418001a56a65f12d66ccad4dc588)

But for our case with the cubic function is enough:

![BezierLaXupax2](https://wikimedia.org/api/rest_v1/media/math/render/svg/199d9916a0917ad0ae914189806693611c585a5b)

Depending on the points that we give to the ecuation, we end up with a path, that we can then interpretate into code to have smooth movements. We can see examples of this on this web pages:

easings.net           |  cubic-bezier.com
:-------------------------:|:-------------------------:
[![Alt Description](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Screenshot%20at%20mar%2030%2001-13-15.png?raw=true)](http://easings.net/es#)|  [![Alt Description](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Screenshot%20at%20mar%2030%2001-12-41.png?raw=true)](http://cubic-bezier.com/#)

 ### 2.4 Implementation
 The implementation it's very similar to in the other cases, and they will be used as actions inside the Action brackets:
```C++
 <act name = "options_window" animation = "movement" movement_type = "smooth" destination_x = "-500" destination_y = "130" time = "0.5">
     <Something here> // This will be executed when the animation finishes.
 </act>
 ```
Depending on the type of movement that we use, we add the prefered points to the bézier equation.

 ## 3. Conclusion
# It's worth?
The high-level summary is that it is possible to automatically generate good quality user interfaces, but only in certain conditions.
The most important trade-off to consider when deciding whether or not to try automatic generation is the amount of information that must be communicated to the automatic system in order for it to produce a good interface design.

## 4. Exercice
The code of the solution and the exercice that you will find it's just only a gideline and it's hard to adapt to another project/type of UI. Use it just as a gideline for the strucuture.

In this exercice you will be creating a basic UI with it's behaviours, using only the XML, as an experience in how fast, easy, difficult, etc.. compared with in-code UI creation.

### Download link
Download exercice and solution: <http://www.mediafire.com/file/xwy95p4us45ffw1/DataDrivenUI.zip>

### TODO 1
Create all and only the UI elements using the blueprints that you can find on the XML.

### TODO 2
Create the basic actions that you can observe on the solution:
- Change of scene.
- Animation of the window when pressing options.

### TODO 3
Create a variable and increase the value of the number when pressing the buttons '+' and '-'.

### Expected:
![BezierLaXupax2](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/Expected.png?raw=true)
### Positions
#### Right
- Main window: position_x = "700" position_y = "200" size_w = "225" size_h = "219"
- Background image: position_x ="700" position_y ="200" rect_x ="0" rect_y =" 0" rect_w ="225"
- Text ("super awesome game"): position_x ="713" position_y = "212" font ="3" spacing ="0"
- Button start: position_x = "717" position_y = "250" size_w = "194" size_h = "49"
    * "over" rect_x ="0" rect_y ="219" rect_w ="190" rect_h ="49"
    * "pressed" rect_x ="196" rect_y ="219" rect_w ="194" rect_h ="49"
    * "idle" rect_x ="395" rect_y ="219" rect_w ="190" rect_h ="49"

- Start game: position_x ="754" position_y = "265" font ="2"
- Button options: position_x = "717" position_y = "310" size_w = "194" size_h = "37"

    * "idle" rect_x ="0" rect_y ="268" rect_w ="190" rect_h ="36"
    * "pressed" rect_x ="196" rect_y ="268" rect_w ="194" rect_h ="36"
    * "over" rect_x ="395" rect_y ="268" rect_w ="190" rect_h ="36"

- Options text: position_x ="775" position_y = "318" font ="2"
- Button credits: position_x = "717" position_y = "360" size_w = "194" size_h = "37"
    * "idle" rect_x ="0" rect_y ="268" rect_w ="190" rect_h ="36"
    * "pressed" rect_x ="196" rect_y ="268" rect_w ="194" rect_h ="36"
    * "over" rect_x ="395" rect_y ="268" rect_w ="190" rect_h ="36"

- Text("credits"): position_x ="775" position_y = "368" font ="2"
  
#### Left
- Main window: position_x = "400" position_y = "130" size_w = "225" size_h = "219"
- Backgroud image: position_x ="400" position_y ="130" rect_x ="0" rect_y =" 304" rect_w ="225" rect_h ="340"
- Text("name"): position_x ="415" position_y = "156" font ="4" spacing ="0"
- Text Back image: position_x ="465" position_y ="150" rect_x ="0" rect_y =" 722" rect_w ="145" rect_h ="28"
- Text input: position_x = "473" position_y = "156" size_w = "133" font = "4"
- Text("cool?"): position_x ="415" position_y = "200" font ="4" spacing ="0"
- Check box: position_x = "470" position_y = "190" size_w = "50" size_h = "50" multiple_choices = "false"
    * "idle" rect_x ="37" rect_y ="778" rect_w ="36" rect_h ="36"
    * "pressed" rect_x ="0" rect_y ="778" rect_w ="36" rect_h ="36"
    * "name" position_x = "470" position_y = "190" size_w = "36" size_h = "36"

- Text("yes"): position_x ="513" position_y = "200" font ="4" spacing ="0"
- Text("how cool?"): position_x ="415" position_y = "250" font ="4" spacing ="0"
- Button add: position_x = "510" position_y = "280" size_w = "36" size_h = "36"
    * "over" rect_x ="0" rect_y ="644" rect_w ="36" rect_h ="36"
    * "pressed" rect_x ="0" rect_y ="644" rect_w ="36" rect_h ="36"
    * "idle" rect_x ="0" rect_y ="644" rect_w ="36" rect_h ="36"
    
- Button substract: position_x = "415" position_y = "280" size_w = "36" size_h = "36"
    * "over" rect_x ="0" rect_y ="680" rect_w ="36" rect_h ="36"
    * "pressed" rect_x ="0" rect_y ="680" rect_w ="36" rect_h ="36"
    * "idle" rect_x ="0" rect_y ="680" rect_w ="36" rect_h ="36"

- Image background number: position_x ="458" position_y ="284" rect_x ="0" rect_y =" 750" rect_w ="45" rect_h ="28"
- Text number: position_x ="473" position_y = "290" font ="4" spacing ="0"
   
