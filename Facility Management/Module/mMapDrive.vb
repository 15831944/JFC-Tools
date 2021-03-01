Imports System.Runtime.InteropServices

Module mMapDrive

    Public Declare Function WNetAddConnection2 Lib "mpr.dll" Alias "WNetAddConnection2A" (ByRef lpNetResource As NETRESOURCE, ByVal lpPassword As String, ByVal lpUserName As String, ByVal dwFlags As Integer) As Integer
    Public Declare Function WNetCancelConnection2 Lib "mpr" Alias "WNetCancelConnection2A" (ByVal lpName As String, ByVal dwFlags As Integer, ByVal fForce As Integer) As Integer
    Public Const ForceDisconnect As Integer = 1
    Public Const RESOURCETYPE_DISK As Long = &H1
    Private Const ERROR_BAD_NETPATH = 53&
    Private Const ERROR_NETWORK_ACCESS_DENIED = 65&
    Private Const ERROR_INVALID_PASSWORD = 86&
    Private Const ERROR_NETWORK_BUSY = 54&

    Public Structure NETRESOURCE
        Public dwScope As Integer
        Public dwType As Integer
        Public dwDisplayType As Integer
        Public dwUsage As Integer
        Public lpLocalName As String
        Public lpRemoteName As String
        Public lpComment As String
        Public lpProvider As String
    End Structure

    'strPath = "\\11.22.33.11\Hostsw\Host\SW\"  
    'When I call MapDrive("T", strpath, "pcs", "$pcspcs$") using the below function it errors with the message "Bad path could not connect to Star Directory".

    Public Function MapDrive(ByVal DriveLetter As String, ByVal UNCPath As String, Optional ByVal strUsername As String = Nothing, Optional ByVal strPassword As String = Nothing) As Boolean

        Dim nr As NETRESOURCE

        nr = New NETRESOURCE With {
            .lpRemoteName = UNCPath,
            .lpLocalName = DriveLetter & ":",
            .lpProvider = Nothing,
            .dwType = RESOURCETYPE_DISK
        }

        Dim result As Integer
        result = WNetAddConnection2(nr, strPassword, strUsername, 0)

        If result = 0 Then
            FormBox.ToolStripStatusLabelFM.Text = Nothing
            Return True
        Else
            Select Case result
                Case ERROR_BAD_NETPATH
                    FormBox.ToolStripStatusLabelFM.Text = "QA4001I Bad path could not connect to Star Directory"
                Case ERROR_INVALID_PASSWORD
                    FormBox.ToolStripStatusLabelFM.Text = "QA4002I Invalid password could not connect to Star Directory"
                Case ERROR_NETWORK_ACCESS_DENIED
                    FormBox.ToolStripStatusLabelFM.Text = "QA4003I Network access denied could not connect to Star Directory"
                Case ERROR_NETWORK_BUSY
                    FormBox.ToolStripStatusLabelFM.Text = "QA4004I Network busy could not connect to Star Directory"
            End Select
            MsgBox("Unable to map drive (" + CStr(result).Trim + ")" + vbNewLine + vbNewLine + "LETTER=" + DriveLetter + vbNewLine + "PATH=" + UNCPath, MsgBoxStyle.Critical)
            Return False
        End If

    End Function

    Public Function UnMapDrive(ByVal DriveLetter As String) As Boolean
        Dim rc As Integer
        rc = WNetCancelConnection2(DriveLetter & ":", 0, ForceDisconnect)

        If rc = 0 Then
            Return True
        Else
            Return False
        End If
    End Function


    'MapDriveNet("x", "\\FoosServer\FoosShare", "FoosServer\Bob", "correcthorsebatterystaple")

    Public Sub MapDriveNetUse(ByVal DriveLetter As String, ByVal UNCPath As String) ', ByVal strUsername As String, ByVal strPassword As String)
        Dim p As New Process()
        p.StartInfo.FileName = "net.exe"
        p.StartInfo.Arguments = " use " & DriveLetter & ": " & UNCPath ' & " " & strPassword & " /USER:" & strUsername
        p.StartInfo.CreateNoWindow = True
        p.Start()
        p.WaitForExit()
    End Sub

End Module
