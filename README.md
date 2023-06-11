# Github
https://github.com/AaronFrans/DAE-Programming4



# Design Choices
Instead of employing the Subject/Observer pattern, I utilized an Event Handler to maintain a distinct segregation of events.
To ensure that inputs are triggered exclusively within their respective scenes, I opted for scene-dependent inputs rather than having them activate across multiple scenes.
To facilitate smooth and convenient file reading and editing, I chose for the use of the JSON format.
# Libraries used
- GLM
- Imgui
- SDL2
    - Image
    - Mixer
    - TTF
- RapidJson

