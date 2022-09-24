# [yacApp]
Yet Another Configurable App

## Object
To provide an easy Configurator to create own and custom Apps

## App and Configurator is One
The App and Configurator are the same Application.
Running the Application without any Parameter starts the Application in App-Mode
Running the Application with Parameter "Configurator" starts the Application in Configurator-Mode

## Idea
If you already have a Website, probably with a Login/Member-Area with this App you can create a downloadable and installable App for your customers an members
Typically you can use your Website in this App in Webviews and connect the Webview-Forms by a custom Menu.
On Sites which require a Login the App forces the User to login and remembers the Login. Therefore a Loginadapter needs to be provided.

## Deploying the App

### Self-hosted with Default-yacApp

### yacApp-hostet with Default-yacApp

### Self-hosted and self-compiled

1. Upload the App-Directory to your Server and make the files available via Webserver
2. Set the URL to <your-server>/global.json in the yacApp - Code
3. Customize other Parts in the yacApp-Code
4. Compile yacApp for Android/iOS
5. Deploy your yacApp to PlayStore and AppStore

### yacApp-hosted and self-compiled

2. Set the URL to <yacApp-server>/global.json in the yacApp - Code
3. Customize other Parts in the yacApp-Code
4. Compile yacApp for Android/iOS
5. Deploy your yacApp to PlayStore and AppStore

[yacApp]: yacApp
