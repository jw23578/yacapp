# [yacApp]
Yet Another Configurable App

## Object
To provide an easy Configurator to create own and custom Apps

## App and Configurator is One
The App and Configurator are the same Application.
Running the Application without any Parameter starts the Application in App-Mode
Running the Application with Parameter "Configurator" starts the Application in Configurator-Mode

## Idea
If you already have a Website, probably with a Login/Member-Area with this App you can create a downloadable and installable App for your customers an members.
Typically you can use your Website in this App in Webviews and connect the Webview-Forms by a custom Menu.
On Sites which require a Login the App forces the User to login and remembers the Login. Therefore a Loginadapter needs to be provided.

## Member/Login-Area
A content page can be configured to need a login.
If ThirdPartyLogin.loginToken is not set then the LoginPage is automatically displayed

Login for now only works with the aidoo (https://www.aidoo.de) online memberarea-login which can be called via directLogin.

If loggedin correctly ThirdPartyLogin.loginToken is set an appended to all webview urls where a login is needed


## Deploying the App

### Self-hosted with Default-[yacApp]

### yacApp-hostet with Default-[yacApp]

### Self-hosted and self-compiled

1. Upload the App-Directory to your Server and make the files available via Webserver
2. Set the URL to <your-server>/<your-project-file>.yacapp in the [yacApp] - Code
3. Customize other Parts in the [yacApp]-Code
4. Compile [yacApp] for Android/iOS
5. Deploy your [yacApp] to PlayStore and AppStore

### yacApp-hosted and self-compiled

2. Set the URL to <yacApp-server>/<your-project-file>.yacapp in the [yacApp] - Code
3. Customize other Parts in the [yacApp]-Code
4. Compile [yacApp] for Android/iOS
5. Deploy your [yacApp] to PlayStore and AppStore

### Current App (early early stage)

yacApp: [Android](https://www.jw78.de/yacapp.apk)

### First Impressions

Configurator: [Video](https://www.jw78.de/yacAppConfigurator.mp4)

App (Running on Linux Desktop): [Video](https://www.jw78.de/yacApp-Desktop.mp4)

[yacApp]: https://github.com/jw23578/yacapp#readme "yacApp"
