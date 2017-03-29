# Research: Data driven ui and animations
## 1. Data driven UI
As we have less things on our projects being configured from code, the better. That applies to console configurations, player starting stats, positions, animations, etc, and also, to UI management and creation. That's when data driven UI begins.
This part of the research is going to talk about the creation and implementation of a UI data driven manager on our code, that will interpretate XML commands, to create full UI systems.

To begin with, we have to determinate the type of UI base system that we are going to use, because that is going to change completly the internal organization of the data manager. Since it's impossibe to create a general data manager without knowing the type of UI that is going to use, this one is going to be based on the UI that i've created on the past. The general structure of this UI is the following:

![UI structure](https://github.com/Guillemsc/Research-Data-driven-ui-and-animations/blob/master/ImagesFolder/UI.png?raw=true)
