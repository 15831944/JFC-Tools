Option Strict On
Option Explicit On

Imports System.Timers

Module mCopyFile

    Private Declare Function apiCopyFile Lib "kernel32" Alias "CopyFileA" (ByVal lpExistingFileName As String, ByVal lpNewFileName As String, ByVal bFailIfExists As Integer) As Integer
    Private Delegate Sub CopyProgressRoutineCallback(ByVal TotalFileSize As Long, ByVal TotalBytesTransferred As Long, ByVal StreamSize As Long, ByVal StreamBytesTransferred As Long, ByVal dwStreamNumber As Integer, ByVal dwCallbackReason As Integer, ByVal hSourceFile As Integer, ByVal hDestinationFile As Integer, ByVal lpData As Integer)
    Private Declare Function CopyFileEx Lib "kernel32.dll" Alias "CopyFileExA" (ByVal lpExistingFileName As String, ByVal lpNewFileName As String, ByVal lpProgressRoutine As CopyProgressRoutineCallback, ByRef lpData As Object, ByRef pbCancel As Integer, ByVal dwCopyFlags As Integer) As Integer

    Private Const PROGRESS_CANCEL As Short = 1 'on annule l'opération (supprime le fichier destination)
    Private Const PROGRESS_CONTINUE As Short = 0 'on continue l'opération
    Private Const PROGRESS_QUIET As Short = 3 'on continue l'opération mais un n'appelant plus la fonction callback
    Private Const PROGRESS_STOP As Short = 2 'on arrête l'opération (peut etre redémarrée)

    Private Const COPY_FILE_FAIL_IF_EXISTS As Integer = &H1
    Private Const COPY_FILE_RESTARTABLE As Integer = &H2

    Public bCancel As Integer

    Dim m_Timer As Double
    Dim m_bProgress As Boolean
    Dim m_Percent As Short

    'Public Shared ReadOnly Property Timer As Double
    '    Get
    '        Return (CDbl((DateTime.Now.Ticks Mod &HC92A69C000)) / 10000000)
    '    End Get
    'End Property

    Public Function CopyFileWithProgress(ByRef Source As String, ByRef Destination As String) As Boolean ', OverwriteDestination As Boolean, Restartable As Boolean) As Long

        Dim OverwriteDestination, Restartable As Boolean
        Dim dwFlags As Integer

        dwFlags = 0
        OverwriteDestination = True
        Restartable = True

        If OverwriteDestination = False Then dwFlags = COPY_FILE_FAIL_IF_EXISTS 'erreur si fichier existe
        If Restartable Then dwFlags = dwFlags Or COPY_FILE_RESTARTABLE 'redémarrable

        m_Timer = Date.Now.TimeOfDay.TotalSeconds 'Microsoft.VisualBasic.Timer
        m_bProgress = False
        m_Percent = 0

        CopyFileWithProgress = CBool(CopyFileEx(Source, Destination, AddressOf CopyProgressRoutine, 0, bCancel, dwFlags))
    End Function

    Private Function CopyProgressRoutine(ByVal TotalFileSize As Long, ByVal TotalBytesTransferred As Long, ByVal StreamSize As Long, ByVal StreamBytesTransferred As Long, ByVal dwStreamNumber As Integer, ByVal dwCallbackReason As Integer, ByVal hSourceFile As Integer, ByVal hDestinationFile As Integer, ByVal lpData As Integer) As Integer

        Dim Percent As Short

        If TotalFileSize > 0 Then

            Percent = CShort(Int((CDbl(TotalBytesTransferred * 10000) / CDbl(TotalFileSize * 10000)) * 100))

            System.Windows.Forms.Application.DoEvents()
            If ((Date.Now.TimeOfDay.TotalSeconds - m_Timer) > 1 And Percent < 50) Or m_bProgress Then
                If Not m_bProgress Then m_bProgress = True
                If Percent > m_Percent Then
                    AUService.CopyFileExProgress((m_Percent))
                    m_Percent = Percent
                End If
            End If

        End If

        'CopyProgressRoutine = PROGRESS_CONTINUE
        'CopyProgressRoutine = PROGRESS_STOP

    End Function

    Public Function CopyFile(ByVal SourceFileName As String, ByVal TargetFileName As String) As Boolean ', ByVal FailIfTargetExists As Boolean) As Long

        Dim strSourceTmp As String = ""
        Dim strTargetTmp As String = ""
        Dim lgFailIfExists As Integer

        Dim FailCopyFile As Integer

        'Modif Olive
        Dim FailIfTargetExists As Boolean = False

        strSourceTmp = SourceFileName & Space(250 - Len(SourceFileName))
        strTargetTmp = TargetFileName & Space(250 - Len(TargetFileName))

        lgFailIfExists = CInt(FailIfTargetExists)

        On Error Resume Next
        FailCopyFile = apiCopyFile(strSourceTmp, strTargetTmp, lgFailIfExists)
        On Error GoTo 0

        If Err.Number <> 0 Then
            MsgBox(Err.Description, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal)
        End If

        CopyFile = CBool(FailCopyFile)

    End Function

End Module