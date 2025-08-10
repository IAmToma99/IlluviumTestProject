IlluviumTestProject

Hi there,
===

# 

# Here is the link to the Git repository for the test project:

# 

# https://github.com/IAmToma99/IlluviumTestProject.git

# 

# To test different scenarios, the following properties can be adjusted:

# Content/Blueprints/BP\_MapGenerator.uasset

# •	Radius

# •	Hex Size

# Content/Blueprints/IlluviumTestGameMode.uasset

# •	Seed

# •	RedUnitsNum

# •	BlueUnitsNum

# •	TimeStepSeconds

# •	MapRadius

# •	HexSize

# 

# &nbsp;Project logic structure:

# •	IlluviumTestGameMode

# &nbsp;       Simulation :

# •	BattleSimulator 

# •	HexCoord

# •	SimUnit

# &nbsp;       Visual :

# •	MapGenerator

# •	VisualUnitActor

# •	Actions

# 

# I implemented the map with a radius of 50 to approximately match the requirements of a 100x100 square grid. Since the camera is set to frame the entire map, elements appear smaller, which reduces visibility.

# For easier visualization, the map size can be reduced together with the camera’s orthographic size:

# 1\.	In the editor, select MapGenerator in the hierarchy. 

# 2\.	Under the MapGenerator section, set Radius to the desire size (e.g. 10 for better visibility)

# 3\.	Press the GenerateMap button in the same section. 

# 4\.	Select the CameraActor in the hierarchy and reduce its orthographic size to improve element visibility. 

# 5\.	Open the IlluviumTestGameMode blueprint and set MapRadius to the same value. 

# 

# Thank you, and feel free to contact me if you have any questions.

# &nbsp;

# 



