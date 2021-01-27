Option Strict On
Option Explicit On

Module mGetWindows

    Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal Hwnd As Integer, ByVal lpString As String, ByVal cch As Integer) As Integer
    Private Delegate Function EnumWindowsCallback(ByVal hWnd As Integer, ByVal lParam As Integer) As Integer
    Private Declare Function EnumWindows Lib "user32.dll" Alias "EnumWindows" (ByVal callback As EnumWindowsCallback, ByVal lParam As Integer) As Integer

	Dim m_sPattern As String
    Dim m_lhFind As Integer

    Private Function EnumWinProc(ByVal Hwnd As Integer, ByVal lParam As Integer) As Integer
        Dim k As Integer
        Dim sName As String
        m_lhFind = 0
        ' Fill buffer
        sName = Space(128)
        ' Get window caption
        k = GetWindowText(Hwnd, sName, 128)
        If k > 0 Then
            ' Trim caption
            sName = Left(sName, k)
            If lParam = 0 Then sName = UCase(sName)
            ' If names match
            If sName Like m_sPattern Then
                ' Return window handle
                m_lhFind = Hwnd
                ' Exit function
                EnumWinProc = 0
                Exit Function
            End If
        End If
        EnumWinProc = 1
    End Function

    Public Function FindWindowHwnd(ByRef sWild As String) As Integer
        ' Save window title
        m_sPattern = UCase(sWild)
        ' enumerate all open windows
        EnumWindows(AddressOf EnumWinProc, CInt(False))
        ' Return window handle
        FindWindowHwnd = m_lhFind
    End Function

    Public Function FindWindow(ByRef TaskWindowsList As String) As Boolean
        Dim TaskMessage As String = ""

        Dim Task() As String
        Dim t As String
        Dim i As Integer

        Task = Split(TaskWindowsList, "|")
        ReDim Preserve Task(20)
        FindWindow = False

        i = 0
        Do
            t = Trim(Task(i))
            If t = "" Then Exit Do
            If CBool(FindWindowHwnd(t)) Then
                If TaskMessage = "" Then
                    TaskMessage = t
                Else
                    TaskMessage = TaskMessage + vbNewLine + t
                End If
                FindWindow = True
            End If
            i += 1
        Loop

        If FindWindow Then
            TaskMessage = "Veulliez quitter l'application suivante:" & vbNewLine & vbNewLine + TaskMessage + vbNewLine + vbNewLine + "puis relancer l'application."
            MsgBox(TaskMessage, MsgBoxStyle.Exclamation)
        End If

    End Function

End Module