# MSP-ProceduralOceanView

## Directly export data from MSP Challenge into Procedural Ocean using POV launcher

1. Start pov-launcher.exe application in the ProceduralOcean and choose "Import POV Config"

![alt text](image-7.png)


select option 2, and use the coordinates you entered. 

![alt text](image-8.png)

You may leave the rest blank/default.

2. Play and have fun, either in Desktop or VR mode.

![alt text](image-9.png)


# Creating and Exporting data from MSP Challenge manually


For this tutorial we will use one of the public servers from MSP Challenge. 


1. Access https://server520.mspchallenge.info/api/doc

![alt text](image-1.png)


2. Log in. You do that by requesting a new user session, with the credentials "user" and "test". Current sessionId for this time of writing is 1. 


![alt text](image-2.png)

You should see the token.

![alt text](image-3.png)

3. Copy the api_access_token without quotes. Scroll up, and click on the green "Authorize" button.

![alt text](image-4.png)

Click authorize, then close.

4. Scroll down to the CreatePOVConfig endpoint. Select the desired coordinates

![alt text](image-5.png)

To find the coordinates you're interested in, go to http://bboxfinder.com/ and use the EPSG:3035 coordinate system. Select the box tool, and copy the coordinates it shows you in the lower-left corner.

![alt text](<Screenshot 2025-09-23 121207-1.png>)

5. Hit Execute. A downloadable .zip file will be offered for download. 

![alt text](image-6.png)

6. If you extract the data into a folder next to the POV launcher exe, the POV launcher will list it to start a Desktop or VR mode.

![alt text](image-7.png)

## Extra steps: Exporting POV config data from MSP

Options are:

1. Use POV launcher to directly import a config into POV data folder, and optional start POV with it. Either use -serverUrl or the interactive menu to set the MSP server to connect to.   
     
   Either use the main server (which is <https://server.mspchallenge.info>) or https://prod-docker.mspchallenge.info. The main server will support POV export from version/release 4.0.1, which is scheduled the 18th of January 2024.   
   https://prod-docker.mspchallenge.info is kept up-to-date with the latest dev code and you do not need to install anything.  
   Finally if you want to use your local MSP server, use <http://localhost.> Chapter "Export using the console command" explains how to install it locally.  
     
   Please follow the chapter "Using POV Launcher" below.
2. Download an export zip directly from the live MSP server (either <https://server.mspchallenge.info> or https://prod-docker.mspchallenge.info) using its API.  
   Please follow the chapter "Export zip using API" below.
3. Install MSPChallenge-Server dev branch locally, and use its API (<http://localhost>), or console command. This also enables you to contribute on development or debugging.  
   Also being able to use the console command enables you to export the config directly to a folder  
   Follow chapter "Export using the console command" if you would like to do that

For all options keep in mind that the MSP game session needs to run at least a single simulation for it to calculate energy distribution. Otherwise it won't export any converter stations or energy cables.

## Extra steps: Using POV Launcher

The POV Launcher can be used from the github repository directly , or from the package build as well.   

![alt text](image.png)
  
There are 2 versions of the POV launcher. Either the script pov-launcher.ps1 itself, or the compiled exe version pov-launcher.exe.   
In the github repository there is only the script at \MSP-ProceduralGeneration\MSP\_ProceduralGen\Scripts\pov-launcher.ps1. You run it by right clicking on it and selecting "Run with Powershell".  
The package build has both pov-launcher.ps1 and pov-launcher.exe.  
I recommend using pov-launcher.exe, since it prevents any issues with PowerShell execution policy.  
Also note that pov-launcher.exe still uses pov-launcher.ps1 to retrieve help information from it.  
  
In-case you get this error running pov-launcher.ps1:  
*"The file* *pov-launcher.ps1 is not digitally signed. You cannot run this script on the current system. For*  
*more information about running scripts and setting execution policy, see about\_Execution\_Policies at*  
*[https:/go.microsoft.com/fwlink/?LinkID=135170](http://https/go.microsoft.com/fwlink/?LinkID=135170).*"  
or "*cannot be loaded because running scripts is disabled*"  
or "*ERROR: Get-Help : Get-Help could not find .\pov-launcher.ps1 in a help file in this session.*"  
You can prevent this - with a one time action - by opening a Powershell with admin rights and running:

`Set-ExecutionPolicy Unrestricted -Scope CurrentUser`

Once you start POV Launcher it will start an interactive menu with the option to "Import POV Config". In case of the package build you can also start POV in desktop/VR either directly or after an import.  
After choosing "Import POV Config" the script will ask you to input the required MSP server url, game session id. region coordinates and the config directory name. There is also the possibility to pass them directly as script command line arguments. You can even set it to automatically import. See the script re-import-all-configs.ps1 as an example, or choose one of the menu options h,g,e to get help, possible command line parameters, or examples.  
The script re-import-all-configs.ps1 can be run to re-import all pov configs used in POV (except Test\_01\_Data which has been made manually).

The export will be directly imported into the POV data folder which is MSP\_ProceduralGen\Content\MSP\_ProceduralOceanView\08\_NonAssets\[configDirName].  
  
The source code of the pov launcher script is maintained here: <https://github.com/BredaUniversityResearch/MSP-ProceduralGeneration/blob/main/MSP_ProceduralGen/Scripts/pov-launcher.ps1>  
<https://github.com/BredaUniversityResearch/MSP-ProceduralGeneration/blob/main/MSP_ProceduralGen/Scripts/re-import-all-configs.ps1>

## Extra steps: Export zip using API

- Open your favorite text editor and copy below lines  
  (double click in the code block to select all lines automatically)

```
```powershell
#### Setup BEGIN ####
# The MSP server URL to use for the POV export
#   for local dev use http://localhost, for shared dev use https://test2.mspchallenge.info, for the main prod use https://server.mspchallenge.info
$serverUrl = "https://server.mspchallenge.info"

# The id of the game session to export config data from
$gameSessionId = 1

# The POV export request data, which is:
#   - region coordinates specified by bottom left x, -y and top right x and -y float values
# Coordinate system: https://epsg.io/3035
$createPOVConfigData = @{
    region_bottom_left_x = "4048536"
    region_bottom_left_y = "3470232"
    region_top_right_x = "4063746"
    region_top_right_y = "3488599"
}
#### Setup END ####

#### Export BEGIN ####
# Lets export
try {
  $response = Invoke-WebRequest -Uri "$serverUrl/$gameSessionId/api/Game/CreatePOVConfig" -Method Post -Body $createPOVConfigData -ContentType "application/x-www-form-urlencoded" -ErrorAction Stop
}
catch { Write-Host "Error: $($_.Exception.Message)" }

#### Export END ####
# Save the binary content to a file
$filename = $response.Headers.'Content-Disposition'.Split('=')[1]
$response.Content | Set-Content -Encoding Byte -Path $filename -Force

# Open the default associated program for the zip file
Start-Process $filename
```
```

- Go through all settings in the "Setup BEGIN" to "Setup END" section, like $serverUrl commented with "The MSP server URL to use for the POV export", and set the correct values for your export.
- Then copy and paste the lines into an open Windows Powershell.
- It will execute the lines and if everything goes to plan , it will download a exported pov config to a zip file and open it up

## Extra steps: Export using the console command

This requires you to **install the MSPChallenge Server locally**. You could follow the extensive MSP  page, but you might as well just follow this shortened version:

1. Install "[Git for Windows](https://gitforwindows.org/)" with default/recommended options and: Uncheck "only show new options". Enable "Git LFS". Set override default branch name to "main". Enable symbolic links.
2. After the install, launch "Git bash" app. Run command:

   `git clone https:``//github.com/BredaUniversityResearch/MSPChallenge-Server.git`
3. This will create a folder MSPChallenge-Server. To open the folder and switch to the dev branch, run command:   
   `cd MSPChallenge-Server && git checkout dev`
4. Install Docker Desktop for Windows from the website: <https://www.docker.com/>. During installation, make sure to select the option to use **WSL 2 based engine**.
5. From the MSPChallenge-Server folder in Git bash, run command:   
   `source docker-aliases.sh`
6. Then enter **dcup** (for a fast production setup), or **dcu** (for the normal dev setup). This will start the Docker container building process and will take a couple of minutes. Once Docker Desktop has a mspchallenge-php-1 container showing a log entry: "GET /index.php" (after "Starting supervisor..."), it is ready to handle your requests.
7. Now open Notepad **as Administrator,** and go to the folder C:\Windows\System32\drivers\etc and open the file hosts. At the bottom of the file, copy/paste the following line:  
   127.0.0.1       caddy mitmproxy database
8. You now should have access to a local Server Manager web app through <http://localhost/ServerManager.> Open it up.
   1. Every time you log on to any installation of the Server Manager, you will need to log in using an MSP Challenge Authoriser account.   
      If you don't have one yet, then you can create one on <https://auth2.mspchallenge.info.>
   2. The first time you ever log in to your own local ServerManager, you should see a 'Server Installation' notice after you've successfully logged in. This creates the default database and registers your ServerManager in the MSP Challenge Authoriser.
   3. You'll then be able to create MSP Challenge game sessions. Start by clicking on New Session, fill in the mandatory fields, set the Admin password to "test" and click create.
9. You created your first game session with id 1. Now you can export a POV config from game session 1 using the API (see chapter above) or console command (see below).

To **export a POV config using the command line**:

1. Open Docker Desktop
2. Go to "Containers", click on "php-1" link. Then choose the tab "Exec" to open a terminal inside the mspchallenge-php-1 container. You should see a prompt in /srv/app.
3. Type the help command:

   ```
   php bin/console app:create-pov-config --help
   ```
4. This will output the help of the command incl. its usage which is:

   ```
   app:create-pov-config [options] [--] <session-id> <region-coordinates>...
   ```
5. Please check the available options you have like -z (to a packaged zip), or some options to change the output directory, json-, zip filename. By default it will created an output directory containing the json config file and a subfolder RasterMaps/ with all the raster files.
6. E.g., to output to the default POV/ folder for game session 1 with some coordinates with some verbosity, run command:

   ```
   php bin/console app:create-pov-config 1 4048536 3470232 4063746 3488599 -v
   ```

   ![image-2023-12-18_22-4-24.png](../attachments/image-2023-12-18_22-4-24.png)  
   Note the region coordinate system used is <https://epsg.io/3035>
7. Notice the OK message , e.g.: Created config in directory: /srv/app/POV/4048536-3470232-4063746-3488599 .   
   Inside the docker container /srv/app is linked to your MSPChallenge-Server folder, so the POV/ folder is a subfolder of MSPChallenge-Server

## Extra steps: Using exported directory config data in POV

It's also possible to **directly use the directory output in POV**'s 08\_NonAssets folder. Either using symlinks or configure the command to output to the 08\_NonAssets folder (but requires you to install php locally):  
  
The easiest way is to **use symlinks**.   
First - as a prerequisite - you need to define a environmental variable called CODEDIR in which your MSPChallenge-Server is installed. In Git bash run the export command to set it. E.g:.

```
export CODEDIR="/d/BUAS/code"
```

Then each time you have exported a new config to the MSPChallenge-Server/POV/ folder, go POV's 08\_NonAssets folder   
(which is MSP-ProceduralGeneration/MSP\_ProceduralGen/Content/MSP\_ProceduralOceanView/08\_NonAssets)  
and then run below command in **Git bash**:

```
find . -type l -delete ; find "$CODEDIR/MSPChallenge-Server/POV/" -mindepth 1 -maxdepth 1 -exec ln -s {} . \;
```

This will create a symlink for each config directory exported into the current folder, which is 08\_NonAssets. It will also remove any previous existing symlinks.   
  
Another way it to actually output exported config directories to POV's 08\_NonAssets folder using the command line option -d. Problem is that the docker container will not be able to access the POV project folder since it not part of the MSP-Challenge server containers. So, there is no way to specify an output path to it from inside docker. However, from the host (your Windows) you can run the command if you have PHP installed. To do so, please follow the chapter called "Tip: Use Powershell PHP manager" on .   
After that you go to the MSPChallenge-Server folder in git bash and run:

```
bash install.sh
```

This will install all prerequisites to be able to run the command from the host. And it should look like this:

![image-2023-12-11_22-9-45.png](../attachments/image-2023-12-11_22-9-45.png)

Then you should be able to run the command with the -d option, e.g. (note the DATABASE\_PORT being specified, which is the port to access the database in docker)

```
DATABASE_PORT=3307 php bin/console app:create-pov-config 1 4048536 3470232 4063746 3488599 -v -d "$CODEDIR/MSP-ProceduralGeneration/MSP_ProceduralGen/Content/MSP_ProceduralOceanView/08_NonAssets/"
```

e.g.:

![image-2023-12-18_22-31-56.png](../attachments/image-2023-12-18_22-31-56.png)
