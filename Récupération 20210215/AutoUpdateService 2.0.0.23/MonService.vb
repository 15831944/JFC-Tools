Option Strict On
Option Explicit On

Imports System.ComponentModel
Imports System.Configuration.Install
Imports System.Net
Imports System.ServiceProcess

<RunInstallerAttribute(True)>
Public Class MyProjectInstaller
    'Cette classe permet de pouvoir compiler le service afin d'en obtenir un executable binaire
    'Cependant, ce n'est pas suffisant pour installer le service windows. Ceci permet juste d'avoir un .exe
    'Il faudra une etape supplementaire pour installer le service sur votre machine
    Inherits Installer
    Private ReadOnly monServiceInstaller As ServiceInstaller
    Private ReadOnly monProcessInstaller As ServiceProcessInstaller

    'Public WithEvents Connection As cConnection

    Public Sub New()
        ' Instantie les installeurs
        monProcessInstaller = New ServiceProcessInstaller
        monServiceInstaller = New ServiceInstaller

        ' The service sera lanc� sous le compte Systeme
        monProcessInstaller.Account = ServiceAccount.LocalSystem

        ' Le service sera demarr� manuellement
        'monServiceInstaller.StartType = ServiceStartMode.Automatic

        'TF1 Publicit�
        monServiceInstaller.StartType = ServiceStartMode.Manual

        'Dim NetWordkLocation As String
        If System.Environment.OSVersion.Version.Major > 5 Then
            'FrameWork 4.0
        Else
            Dim DependedOn() As String = {"Netman", "Nla"}
            monServiceInstaller.ServicesDependedOn = DependedOn
        End If

        ' Le nom du service doit etre egal au nom de la classe ServiceBase dont on derive (voir le  Public Sub New() de la classe MonService pour comprendre)
        monServiceInstaller.ServiceName = "JFC Update"
        monServiceInstaller.DisplayName = "JFC Update"

        monServiceInstaller.Description = "T�l�chargement de mises � jour JFC." '"Download updates JFC"

        ' Ajouter les installeurs � la collection (l'ordre n'est pas important)
        Installers.Add(monServiceInstaller)
        Installers.Add(monProcessInstaller)
    End Sub
End Class

Public Class MonService
    Inherits System.ServiceProcess.ServiceBase

    Public Shared AppPath As String = My.Application.Info.DirectoryPath + "\"

    Public Shared svServiceIni As String = AppPath + My.Application.Info.AssemblyName + ".ini"

    Public Shared HostName As String = Dns.GetHostEntry(Dns.GetHostName).HostName

    Public Shared MaConnection As CConnection = New CConnection

    Public Shared TimeOut As Integer

    Public Shared DelayReConnect As Integer

    Public Shared DelayRefresh As Integer

    Public Shared DelayService As Integer

    Public Shared DisplayNameService As String = "JFC Update"

    Public Shared DisplayNameReStart As String = "ReStart"

    Public Shared svCompanyNameService As String

    Public Shared AppName As String

    Public Shared AppVersion As String

    Structure Service
        Dim Number As Integer
        Dim CompanyName As String
        Dim Cle As String
        Dim LocalDir As String
        Dim NameAppli As String
        Dim RemoteDir As String
        Dim CodeAppli As String
        Dim Patch As String
        Dim PatchCopy As String
        Dim Execute As String
        Dim CommandLine As String
        Dim CurrentDir As String
        Dim Wait As Boolean
        Dim DeleteAfter As Boolean
    End Structure

    Public Shared Cle As String
    Public Shared LocalDir As String
    Public Shared Patch As String
    Public Shared CompanyName As String

    Dim MonDelegate As CDelegate

    Dim Services() As Service

    Public Sub New()
        MyBase.New()

        InitializeComponent()

        'Ici le nom doit etre identique � celui donn� plus haut ! 
        ServiceName = MonService.DisplayNameService
    End Sub

    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    <MTAThread()>
    Shared Sub Main()
        Dim ServicesToRun() As System.ServiceProcess.ServiceBase

        ServicesToRun = New System.ServiceProcess.ServiceBase() {New MonService}

        System.ServiceProcess.ServiceBase.Run(ServicesToRun)
    End Sub

    Private components As System.ComponentModel.IContainer


    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        components = New System.ComponentModel.Container
        Me.ServiceName = DisplayNameService
    End Sub

    Protected Overrides Sub OnStart(ByVal args() As String)
        ' Code lanc� au demarrage du service
        'faire une action 

        Try

            If IO.File.Exists(AppPath + My.Application.Info.AssemblyName + ".old") Then
                IO.File.Delete(AppPath + My.Application.Info.AssemblyName + ".old")
            End If

            If IO.File.Exists(AppPath + DisplayNameReStart + ".old") Then
                IO.File.Delete(AppPath + DisplayNameReStart + ".old")
            End If

            CompanyName = Select_GetIniString("Parametres", "CompanyName", MonService.svServiceIni)
            svCompanyNameService = Url_Encode(CompanyName)

            InitServer()

            ' Choix de la langue
            Dim svLanguage As String = UCase(Select_GetIniString("Parametres", "Language", MonService.svServiceIni))
            If svLanguage = "EN" Then
                SetCurrentLanguage(0)
            ElseIf svLanguage = "FR" Then
                SetCurrentLanguage(1)
                'ElseIf svLanguage = "IT" Then
                '    SetCurrentLanguage(2)
                'ElseIf svLanguage = "PL" Then
                '    SetCurrentLanguage(3)
            Else 'Default FR
                SetCurrentLanguage(1)
            End If

            AppName = Select_GetIniString("Parametres", "ServiceName", MonService.svServiceIni)

            'Ajout de la version dans l'ini
            Dim AppBuild As String = Select_GetIniString("Parametres", "AppBuild", MonService.svServiceIni)

            Dim AppBuildCurrent As String = My.Application.Info.Version.ToString
            If AppBuild <> AppBuildCurrent Then
                AppBuild = AppBuildCurrent
                Select_WriteIniString("Parametres", "AppBuild", AppBuild, MonService.svServiceIni)
            End If

            Dim AppVersionCurrent As String = FileVersionInfo.GetVersionInfo(AppPath + My.Application.Info.AssemblyName + ".exe").FileVersion
            If AppVersion <> AppVersionCurrent Then
                AppVersion = AppVersionCurrent
                Select_WriteIniString("Parametres", "AppVersion", AppVersion, MonService.svServiceIni)
            End If

            If Not InitService() Then Exit Try

            MonDelegate = New CDelegate
            MonDelegate.Main(Services)

        Catch ex As Exception

        End Try

    End Sub

    Private Function Url_Encode(ByVal svUrl As String) As String

        svUrl = svUrl.Replace(" ", "%20")
        svUrl = svUrl.Replace("!", "%21")
        svUrl = svUrl.Replace("$", "%24")
        svUrl = svUrl.Replace("&", "%26")
        svUrl = svUrl.Replace("'", "%27")
        svUrl = svUrl.Replace("(", "%28")
        svUrl = svUrl.Replace(")", "%29")
        svUrl = svUrl.Replace("@", "%40")
        svUrl = svUrl.Replace("�", "%C0")
        svUrl = svUrl.Replace("�", "%C1")
        svUrl = svUrl.Replace("�", "%C2")
        svUrl = svUrl.Replace("�", "%C3")
        svUrl = svUrl.Replace("�", "%C4")
        svUrl = svUrl.Replace("�", "%C5")
        svUrl = svUrl.Replace("�", "%C6")
        svUrl = svUrl.Replace("�", "%C7")
        svUrl = svUrl.Replace("�", "%C8")
        svUrl = svUrl.Replace("�", "%C9")
        svUrl = svUrl.Replace("�", "%CA")
        svUrl = svUrl.Replace("�", "%CB")
        svUrl = svUrl.Replace("�", "%CC")
        svUrl = svUrl.Replace("�", "%CD")
        svUrl = svUrl.Replace("�", "%CE")
        svUrl = svUrl.Replace("�", "%CF")
        svUrl = svUrl.Replace("�", "%D0")
        svUrl = svUrl.Replace("�", "%D1")
        svUrl = svUrl.Replace("�", "%D2")
        svUrl = svUrl.Replace("�", "%D3")
        svUrl = svUrl.Replace("�", "%D4")
        svUrl = svUrl.Replace("�", "%D5")
        svUrl = svUrl.Replace("�", "%D6")
        svUrl = svUrl.Replace("�", "%D7")
        svUrl = svUrl.Replace("�", "%D9")
        svUrl = svUrl.Replace("�", "%DA")
        svUrl = svUrl.Replace("�", "%DB")
        svUrl = svUrl.Replace("�", "%DC")
        svUrl = svUrl.Replace("�", "%DD")
        svUrl = svUrl.Replace("�", "%E0")
        svUrl = svUrl.Replace("�", "%E1")
        svUrl = svUrl.Replace("�", "%E2")
        svUrl = svUrl.Replace("�", "%E3")
        svUrl = svUrl.Replace("�", "%E4")
        svUrl = svUrl.Replace("�", "%E5")
        svUrl = svUrl.Replace("�", "%E6")
        svUrl = svUrl.Replace("�", "%E7")
        svUrl = svUrl.Replace("�", "%E8")
        svUrl = svUrl.Replace("�", "%E9")
        svUrl = svUrl.Replace("�", "%EA")
        svUrl = svUrl.Replace("�", "%EB")
        svUrl = svUrl.Replace("�", "%EC")
        svUrl = svUrl.Replace("�", "%ED")
        svUrl = svUrl.Replace("�", "%EE")
        svUrl = svUrl.Replace("�", "%EF")
        svUrl = svUrl.Replace("�", "%F0")
        svUrl = svUrl.Replace("�", "%F1")
        svUrl = svUrl.Replace("�", "%F2")
        svUrl = svUrl.Replace("�", "%F3")
        svUrl = svUrl.Replace("�", "%F4")
        svUrl = svUrl.Replace("�", "%F5")
        svUrl = svUrl.Replace("�", "%F6")
        svUrl = svUrl.Replace("�", "%F9")
        svUrl = svUrl.Replace("�", "%FA")
        svUrl = svUrl.Replace("�", "%FB")
        svUrl = svUrl.Replace("�", "%FC")
        svUrl = svUrl.Replace("�", "%FD")
        svUrl = svUrl.Replace("�", "%FE")
        svUrl = svUrl.Replace("�", "%FF")

        Return svUrl

    End Function

    Private Function InitService() As Boolean

        Dim NumOfServ As Integer = 0
        Dim Servive As Integer = 1

        Do

            Dim svCompanyName As String = Select_GetIniString("Service" + CStr(Servive).Trim, "CompanyName", MonService.svServiceIni)
            If svCompanyName = Nothing Then Exit Do

            Dim Cle As String = Select_GetIniString("Service" + CStr(Servive).Trim, "Key", svServiceIni)
            Dim CleFound As String = AppendString(Crypter(svCompanyName, "sha512"))

            If CleFound = Cle Then

                ReDim Preserve Services(NumOfServ)
                Services(NumOfServ).Number = NumOfServ
                Services(NumOfServ).CompanyName = svCompanyName
                Services(NumOfServ).RemoteDir = Select_GetIniString("Service" + CStr(Servive).Trim, "RemoteDir", MonService.svServiceIni)
                Services(NumOfServ).LocalDir = StrAddLastSlash(Select_GetIniString("Service" + CStr(Servive).Trim, "LocalDir", MonService.svServiceIni))
                Services(NumOfServ).NameAppli = Select_GetIniString("Service" + CStr(Servive).Trim, "NameAppli", MonService.svServiceIni)
                Services(NumOfServ).CodeAppli = Select_GetIniString("Service" + CStr(Servive).Trim, "CodeAppli", MonService.svServiceIni)
                Services(NumOfServ).Patch = Select_GetIniString("Service" + CStr(Servive).Trim, "Patch", MonService.svServiceIni)
                Services(NumOfServ).PatchCopy = StrAddLastSlash(Select_GetIniString("Service" + CStr(Servive).Trim, "PatchCopy", MonService.svServiceIni))
                Services(NumOfServ).Execute = Select_GetIniString("Service" + CStr(Servive).Trim, "Execute", MonService.svServiceIni)
                Services(NumOfServ).CommandLine = Select_GetIniString("Service" + CStr(Servive).Trim, "CommandLine", MonService.svServiceIni)
                Services(NumOfServ).CurrentDir = Select_GetIniString("Service" + CStr(Servive).Trim, "CurrentDir", MonService.svServiceIni)
                Services(NumOfServ).Wait = (Select_GetIniString("Service" + CStr(Servive).Trim, "Wait", MonService.svServiceIni) = "1")
                Services(NumOfServ).DeleteAfter = (Select_GetIniString("Service" + CStr(Servive).Trim, "DeleteAfter", MonService.svServiceIni) = "1")
                NumOfServ += 1

            End If

            Servive += 1

        Loop

        InitService = (Services.Length <> 0)

        If InitService Then

            TimeOut = DelayDefault("TimeOut", 300) ' 5 Minutes
            DelayReConnect = DelayDefault("DelayReConnect", 60) ' 60 Secondes
            DelayRefresh = DelayDefault("DelayRefresh", 300) ' 5 Minutes
            DelayService = DelayDefault("DelayService", 5) ' 5 Secondes

        End If

    End Function

    Private Function DelayDefault(ByVal szKey As String, ByVal nvTimeDefault As Integer) As Integer

        Try

            Dim svResult As String = Select_GetIniString("Parametres", szKey, MonService.svServiceIni).Trim

            If svResult = "" Then
                Return nvTimeDefault
            Else
                Return CInt(svResult)
            End If

        Catch ex As Exception
            Return nvTimeDefault
        End Try

    End Function

    Private Function FormatString2Web(ByVal szString As String) As String

        FormatString2Web = Replace(szString, "&", "%26", , , CompareMethod.Text)
        FormatString2Web = Replace(FormatString2Web, " ", "%20", , , CompareMethod.Text)

    End Function

    Private Sub InitServer()

        Dim InternetBuffer As String = Select_GetIniString("Parametres", "InternetBuffer", MonService.svServiceIni)
        Dim InternetFlagsMask As String = Select_GetIniString("Parametres", "InternetFlagsMask", MonService.svServiceIni)

        With MonService.MaConnection
            .Proxy.Proxy = Select_GetIniString("Parametres", "Proxy", MonService.svServiceIni)
            .DirectConnect = (Select_GetIniString("Parametres", "DirectConnect", MonService.svServiceIni) = "1")
            .PassiveMode = (Select_GetIniString("Parametres", "PassiveMode", MonService.svServiceIni) = "1")
            .DebugMode = (Select_GetIniString("Parametres", "Debug", MonService.svServiceIni) = "1")
            .CodeAppli = Select_GetIniString("Parametres", "CodeAppli", MonService.svServiceIni)
            .RemoteFileAcces = FormatString2Web(MonService.CompanyName) & "." & AppendString(Crypter(MonService.CompanyName, "sha1"))

            .UserName = Select_GetIniString("Parametres", "UserName", MonService.svServiceIni).Trim
            .Password = Select_GetIniString("Parametres", "Password", MonService.svServiceIni).Trim

            .RemoteDir = Select_GetIniString("Parametres", "RemoteDir", MonService.svServiceIni)

            .ServerName = Select_GetIniString("Parametres", "ServerName", MonService.svServiceIni)

            If .Proxy.Proxy <> vbNullString Then
                .Proxy.InitProxy()
            End If

            If InternetBuffer = Nothing Then
                .InternetBuffer = 8
            Else
                .InternetBuffer = CShort(InternetBuffer)
            End If

            If InternetFlagsMask = Nothing Then
                .InternetFlagsMask = 0
            Else
                .InternetFlagsMask = CInt(InternetFlagsMask)
            End If

        End With

    End Sub

    Protected Overrides Sub OnStop()
        ' Code qui sert � effectuer des operations de "fermeture" avant la fin du service
        If MonDelegate IsNot Nothing Then
            MonDelegate.StopThread()
        End If

    End Sub

End Class
