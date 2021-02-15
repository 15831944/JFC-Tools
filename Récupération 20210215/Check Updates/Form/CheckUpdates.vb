'ADD NEW

Option Strict On
Option Explicit On

Imports System.Data.SqlClient
Imports VB = Microsoft.VisualBasic

Public Class CheckUpdates

    Private Shared nvCount As Integer = 0
    Private Shared exitflag As Boolean = True
    Private Shared nvColor As Integer = 0

    Public Const DIRECTORY As Short = 1 'Indicates that the path minus the disk drive letter and filename should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the path without the server and shared device name, and without the filename if one was specified. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \TheApp\. 
    Public Const DISK As Short = 2 'Indicates that the disk drive designation (drive letter followed by a colon) should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the server and shared device name. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice. 
    Public Const EXTENSION_ONLY As Short = 3 'Indicates that the file extension should be returned in svReturnString. It does not include the period.
    Public Const FILENAME As Short = 4 'Indicates that the complete filename (that is, with its file extension) should be returned in svReturnString.
    Public Const FILENAME_ONLY As Short = 5 'Indicates that the filename only (that is, without its file extension) should be returned in svReturnString.
    Public Const PATH As Short = 6 'Indicates that the path minus the filename should be returned in svReturnString. This option differs from DIRECTORY in that the drive designation (if specified in szPath) is included in the returned path. When szPath specifies a UNC path the server and shared device name are included in the returned path. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice\TheApp\.
    Public Const PATH_FILENAME_ONLY As Short = 7

    Public DicSoft As New Dictionary(Of String, String)
    Public cMSDic As New Dictionary(Of String, Integer)

    Public WithEvents lb As New ListBoxEX

    Friend WithEvents Label As System.Windows.Forms.Label
    Friend WithEvents PictureBox As System.Windows.Forms.PictureBox

    Dim Acces As New Dictionary(Of String, String)
    Dim connectString As String
    Dim connection As SqlConnection

    Dim CheckUpdatesIni As String

    Dim TabFilterDisable() As String

    Dim svStartFind As String

    Dim mPath As String
    Dim mLogiciel As String

    Dim bCancel As Boolean = False
    Dim bStopProcess As Boolean = False

    Dim ListNotUpdate As String = Nothing

    Structure STATUS_CLIENT

        Dim image As System.Drawing.Image
        Dim value As Integer

    End Structure

    Private Sub SetColor()

        Dim MeColorBack As System.Drawing.Color = New System.Drawing.Color
        Dim ButtonColorBack As System.Drawing.Color = New System.Drawing.Color
        Dim FlatAppearance As System.Drawing.Color = New System.Drawing.Color


        MeColorBack = System.Drawing.Color.FromArgb(191, 219, 255)
        ButtonColorBack = System.Drawing.Color.FromArgb(191, 219, 255)
        FlatAppearance = System.Drawing.Color.FromArgb(191, 219, 255)

        'ButtonColorBack = System.Drawing.Color.FromArgb(220, 230, 238)
        'FlatAppearance = System.Drawing.Color.FromArgb(170, 200, 210)

        Me.BackColor = MeColorBack
        ButtonExit.BackColor = ButtonColorBack

        'Logiciel.BackColor = ButtonColorBack

        ProgressBar1.BackColor = ButtonColorBack
        ProgressBar1.ForeColor = FlatAppearance

        ButtonExit.FlatAppearance.MouseOverBackColor = FlatAppearance

        ButtonExit.FlatAppearance.MouseDownBackColor = FlatAppearance


        Logiciel.Font = New Font("Tahoma", 18, FontStyle.Bold, GraphicsUnit.Pixel)
        Logiciel.ForeColor = Color.DimGray


    End Sub

    Private Sub lb_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles lb.DoubleClick

        Disabled_Form()

        Refresh_Client()

        Enabled_Form()

    End Sub

    Private Sub Refresh_Client()

        Dim it As ListBoxEX.ItemEX = lb.Items(lb.SelectedIndex)

        Dim nvUpdate1Index As Integer = 0
        Dim nvUpdate2Index As Integer = 0

        Dim svUpdate1Index As String = Nothing
        Dim svUpdate2Index As String = Nothing

        FiltreClients(it.Text, nvUpdate1Index, nvUpdate2Index, svUpdate1Index, svUpdate2Index, TabFilterDisable.Contains(it.Text), True)

        If ListNotUpdate <> Nothing Then
            MsgBox(ListNotUpdate, MsgBoxStyle.Exclamation)
        End If
        lb.Replace(lb.SelectedIndex, svUpdate1Index + " / " + svUpdate2Index, it.Text, nvUpdate1Index, nvUpdate2Index)

    End Sub

    Private Sub ClicDroit_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles ClicDroit.ItemClicked

        Disabled_Form()

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        If str <> Nothing Then
            str = str
        End If

        Select Case int

            Case 9 'Refresh
                Refresh_Client()

            Case 10 'Exclude
                Dim index As Integer = lb.SelectedIndexItem

                Dim svFilterExclude As String = Select_GetIniString(Logiciel.Text, "Exclude", CheckUpdatesIni).Trim

                If svFilterExclude = Nothing Then
                    svFilterExclude = lb.Items(index).Text
                Else
                    svFilterExclude = svFilterExclude + ";" + lb.Items(index).Text
                End If
                lb.IndexRemove(index)
                Select_WriteIniString(Logiciel.Text, "Exclude", svFilterExclude, CheckUpdatesIni)

            Case 11 'Reset

                bStopProcess = False

                Select_WriteIniString(Logiciel.Text, "Exclude", "", CheckUpdatesIni)
                lb.Clear()

                Directory_Acces()


        End Select

        Enabled_Form()


    End Sub

    Private Sub Create_ListBoxEx()

        lb.AddImage(My.Resources.Resources.vert) '0
        lb.AddImage(My.Resources.Resources.jaune) '1 
        lb.AddImage(My.Resources.Resources.orange) '2
        lb.AddImage(My.Resources.Resources.rouge) '3
        lb.AddImage(My.Resources.Resources.gris) '4 
        lb.AddImage(My.Resources.Resources.bleu) '5

        lb.Location = New Point(Logiciel.Location.X, Logiciel.Location.Y + Logiciel.Size.Height + Logiciel.Margin.Top)

        lb.Width = Logiciel.Size.Width
        lb.Height = ButtonExit.Location.Y - lb.Location.Y - Logiciel.Margin.Top

        System.Windows.Forms.Application.DoEvents()

        Me.Controls.Add(lb)

    End Sub

    Private Sub FiltreClients(ByVal obj As String, ByRef nvUpdate1Index As Integer, ByRef nvUpdate2Index As Integer, ByRef svUpdate1Index As String, ByRef svUpdate2Index As String, Optional ByVal bDisable As Boolean = False, Optional ByVal bInfo As Boolean = False)

        Dim bStartZero As Boolean = False

ReStart:

        If bDisable Then
            nvUpdate1Index = -1
        Else
            nvUpdate1Index = StatusClient(obj, "1", , bStartZero)
            If nvUpdate1Index < 0 And bStartZero = False Then bStartZero = True : GoTo ReStart
        End If

        Dim svNewsName As String = Select_GetIniString(Logiciel.Text, obj, CheckUpdatesIni).Trim
        nvUpdate2Index = StatusClient(obj, "2", svNewsName, bInfo, bStartZero)
        If nvUpdate2Index < 0 And bStartZero = False Then bStartZero = True : GoTo ReStart

        If nvUpdate1Index < 0 Then
            svUpdate1Index = "?"
            nvUpdate1Index = 4
        ElseIf nvUpdate1Index > 0 Then
            svUpdate1Index = CStr(-nvUpdate1Index)
            If nvUpdate1Index > 2 Then
                nvUpdate1Index = 3
            End If
        ElseIf nvUpdate1Index = 0 Then
            svUpdate1Index = "0"
        End If

        If nvUpdate2Index < 0 Then
            svUpdate2Index = "?"
            nvUpdate2Index = 4
        ElseIf nvUpdate2Index > 0 Then
            svUpdate2Index = CStr(-nvUpdate2Index)
            If nvUpdate2Index > 2 Then
                nvUpdate2Index = 3
            End If
        ElseIf nvUpdate2Index = 0 Then
            svUpdate2Index = "0"
        End If


    End Sub

    Private Sub Directory_Acces()

        Dim ListAcces() As String = System.IO.Directory.GetFiles(mPath, "*.acces", System.IO.SearchOption.TopDirectoryOnly)

        ListAcces = (From x In ListAcces Select x Order By x Ascending).ToArray
        ListAcces = (From x In ListAcces Select ParsePath(x, FILENAME_ONLY)).ToArray()

        Dim nvCount As Integer = 0

        ProgressBar1.Maximum = ListAcces.Length
        ProgressBar1.Value = nvCount

        Dim svFilterExclude As String = Select_GetIniString(Logiciel.Text, "Exclude", CheckUpdatesIni).Trim

        Dim TabFilterExclude() As String = Split(svFilterExclude, ";")

        Dim svFilterDisable As String = Select_GetIniString(Logiciel.Text, "Disable", CheckUpdatesIni).Trim

        TabFilterDisable = Split(svFilterDisable, ";")

        System.Windows.Forms.Application.DoEvents()

        For Each obj In ListAcces

            If bStopProcess Then Exit For

            Dim nvUpdate1Index As Integer = 0
            Dim nvUpdate2Index As Integer = 0

            Dim svUpdate1Index As String = Nothing
            Dim svUpdate2Index As String = Nothing


            If Not TabFilterExclude.Contains(obj) Then

                FiltreClients(obj, nvUpdate1Index, nvUpdate2Index, svUpdate1Index, svUpdate2Index, TabFilterDisable.Contains(obj))

                lb.Add(svUpdate1Index + " / " + svUpdate2Index, obj, nvUpdate1Index, nvUpdate2Index)

                lb.IndexItem = nvCount

                nvCount = nvCount + 1

            Else

                ProgressBar1.Maximum = ProgressBar1.Maximum - 1

            End If

            ProgressBar1.Value = nvCount

            System.Windows.Forms.Application.DoEvents()

        Next

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Public Function ParsePath(ByVal szPath As String, ByVal nOperation As Short) As String

        Dim nPosSlash, nPosPoint, nLenPath As Integer

        nPosPoint = InStrRev(szPath, ".") - 1
        nPosSlash = InStrRev(szPath, "\")
        nLenPath = Len(szPath)

        If nOperation = EXTENSION_ONLY Then
            Return VB.Right(szPath, nLenPath - nPosPoint)
        ElseIf nOperation = FILENAME_ONLY Then
            Return Mid(szPath, nPosSlash + 1, nPosPoint - nPosSlash)
        ElseIf nOperation = FILENAME Then
            Return VB.Right(szPath, nLenPath - nPosSlash)
        ElseIf nOperation = PATH Then
            Return VB.Left(szPath, nPosSlash)
        ElseIf nOperation = PATH_FILENAME_ONLY Then
            Return VB.Left(szPath, nPosPoint)
        Else
            Return szPath
        End If


    End Function

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        SetColor()

        Me.Opacity = 0.95

        Me.Size = CType(New Point(400, 400), Drawing.Size)

        ' Définition d'un chemin de fichier pour d'autre extension
        Dim CheckUpdatesPath As String = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY)

        ' Fichier INI de paramétrages
        CheckUpdatesIni = CheckUpdatesPath & ".ini"

        'Serveur SQL
        Dim SQL_Serveur As String = Select_GetIniString("SQL", "Server", CheckUpdatesIni).Trim
        Dim SQL_Login As String = Select_GetIniString("SQL", "Login", CheckUpdatesIni).Trim
        Dim SQL_Password As String = Select_GetIniString("SQL", "Password", CheckUpdatesIni).Trim

        Try
            'Chaîne de connexion
            connectString = "database=siteinternet;server=" + SQL_Serveur + ";User ID=" + SQL_Login + ";pwd=" + SQL_Password

            'Objet connection
            connection = New SqlConnection(connectString)

            'Ouverture
            connection.Open()

        Catch ex As Exception

            MsgBox("Problem connecting to the SQL database!", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), "Check Updates")
            'Me.Close()

        End Try

        DicSoft.Clear()

        Dim mPathExes As String = Select_GetIniString("Parametres", "PathDataFret", CheckUpdatesIni).Trim
        Dim Software() As String = Split(Select_GetIniString("Parametres", "Software", CheckUpdatesIni), ";")
        For a = 0 To Software.Length - 1
            DicSoft.Add(Software(a).Trim, mPathExes + Select_GetIniString(Software(a).Trim, "Path", CheckUpdatesIni))
        Next

        For Each obj In DicSoft.Keys
            Logiciel.Items.Add(obj)
        Next

        Create_ListBoxEx()


        cMSDic.Add("New/Created", 0)
        cMSDic.Add("Edit selection", 1)
        cMSDic.Add("Delete selection", 2)
        cMSDic.Add("Add selection", 3)
        cMSDic.Add("Add all", 4)
        cMSDic.Add("Remove selection", 5)
        cMSDic.Add("Remove all", 6)
        cMSDic.Add("Search in customer", 7)
        cMSDic.Add("Duplicate customer", 8)
        cMSDic.Add("Refresh customer", 9)
        cMSDic.Add("Exclude", 10)
        cMSDic.Add("Reset", 11)

        svStartFind = Select_GetIniString("Parametres", "StartFind", CheckUpdatesIni).Trim


        lb.ContextMenuStrip = ClicDroit
        'ClicDroit.Items.Add("Refresh")
        ClicDroit.Font = New System.Drawing.Font("Tahoma", 11.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))

        ClicDroit.Items.Add(cMSDic.Keys(9), My.Resources.refresh)
        ClicDroit.Items.Item(ClicDroit.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        ClicDroit.Items.Add(cMSDic.Keys(10), My.Resources.stop1)
        ClicDroit.Items.Item(ClicDroit.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        ClicDroit.Items.Add(cMSDic.Keys(11), My.Resources.refresh2)
        ClicDroit.Items.Item(ClicDroit.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        lb.Enabled = False

    End Sub

    Private Function ReadSql(ByVal svClient As String, ByVal svPatch As String, Optional ByVal etat As String = Nothing, Optional ByVal bStartZero As Boolean = False) As Boolean

        If etat = Nothing Then etat = "5"

        Try
            ''date time' like '%2010%' and

            '***********************************************
            'Modif du 25/04/2017 Problème avec Affinage Auto
            '***********************************************
            Dim svLogiciel As String = mLogiciel
            If svClient.Contains("Affinage") And mLogiciel = "Atelier Radio" Then
                svLogiciel = "AffinageAuto"
            End If
            '***********************************************

            '***********************************************
            'Modif du 27/04/2017 Accélération du process
            '***********************************************
            Dim szStartFind As String = svStartFind
            If bStartZero Then
                szStartFind = "0"
            End If
            '***********************************************

            'Modif du 25/04/2017 Problème d'accents
            Dim command As SqlCommand = New SqlCommand("Select DISTINCT maj from Log where  login = '" + svClient.Replace("é", "?").Replace("è", "?") + "' and  maj like '%" + svPatch + "' and logiciel = '" + svLogiciel + "' and etat = '" + etat + "' and id > '" + szStartFind + "'", connection)

            'Object datareader
            Dim reader As SqlDataReader = command.ExecuteReader()

            ReadSql = reader.HasRows

            'Fermeture reader
            reader.Close()

        Catch ex As Exception

            ReadSql = False

            MsgBox("Problem connecting to the SQL database!", CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), "Check Updates")
            Me.Close()

        End Try

    End Function

    Private Sub ButtonExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonExit.Click

        If Not bCancel Then
            'Fermeture base
            connection.Close()
            Me.Close()
            End
        Else
            bStopProcess = True
        End If

    End Sub

    Public Function StatusClient(ByVal sAcces As String, Optional ByVal sEtat As String = Nothing, Optional ByVal sClient As String = Nothing, Optional ByVal binfo As Boolean = False, Optional ByVal bStartZero As Boolean = False) As Integer

        ListNotUpdate = Nothing

        Dim svFile As String = mPath + sAcces + ".acces"

        If sClient = Nothing Then
            sClient = sAcces
        End If

        If IO.File.Exists(svFile) Then

            Dim lPatch() As String = IO.File.ReadAllLines(svFile, System.Text.Encoding.Default)

            Dim ListOfUpdate As ArrayList = New ArrayList
            ListOfUpdate.Clear()
            ListOfUpdate.AddRange(lPatch)
            Dim IndexListEnd As Integer = ListOfUpdate.IndexOf("##")

            If IndexListEnd <= 0 Then Return 0

            ReDim Preserve lPatch(IndexListEnd - 1)
            Dim sPatch As String = lPatch(lPatch.Length - 1)

            Dim sTab() As String = Split(lPatch(lPatch.Length - 1), ",")
            sPatch = sTab(1)

            Dim nvCount As Integer = 0
            Try

                While Not ReadSql(sClient, sPatch, sEtat, bStartZero)

                    System.Windows.Forms.Application.DoEvents()

                    nvCount = nvCount + 1

                    If nvCount > lPatch.Length - 1 Then Return -1

                    sPatch = lPatch(lPatch.Length - nvCount - 1)

                    sTab = Split(sPatch, ",")
                    If binfo Then
                        If sEtat = "2" Then
                            ListNotUpdate = sTab(4).Replace("\r\n\r\n", "\r\n").Replace("\r\n", vbNewLine) + CStr(IIf(ListNotUpdate <> Nothing, vbNewLine + vbNewLine + ListNotUpdate, Nothing))
                        End If
                    End If

                    sPatch = sTab(1)

                End While

            Catch ex As Exception
                'MsgBox(CStr(nvCount) + vbNewLine + sPatch + vbNewLine + CStr(lPatch.Length), MsgBoxStyle.Critical)
                Return -1
            End Try

            Return nvCount

        Else
            Return 0
        End If

    End Function

    Private Sub Logiciel_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Logiciel.SelectedIndexChanged

        Disabled_Form()

        bStopProcess = False

        mLogiciel = Logiciel.Text

        mPath = DicSoft(mLogiciel)

        mLogiciel = Replace(mLogiciel, "(RU)", "").Trim
        mLogiciel = Replace(mLogiciel, "(FR)", "").Trim
        mLogiciel = Replace(mLogiciel, "(BE)", "").Trim
        mLogiciel = Replace(mLogiciel, "(IT)", "").Trim
        mLogiciel = Replace(mLogiciel, "(UK)", "").Trim
        mLogiciel = Replace(mLogiciel, "(NO)", "").Trim

        lb.Clear()

        Directory_Acces()

        Enabled_Form()

    End Sub

    Private Sub Enabled_Form()

        Logiciel.Enabled = True
        'ButtonExit.Enabled = True
        lb.Enabled = True

        bCancel = False

        ButtonExit.Text = "&Exit"

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub Disabled_Form()

        Logiciel.Enabled = False
        'ButtonExit.Enabled = False
        lb.Enabled = False

        bCancel = True

        ButtonExit.Text = "&Cancel"

        System.Windows.Forms.Application.DoEvents()

    End Sub

    'Private Sub ClicDroit_Opened(sender As Object, e As System.EventArgs) Handles ClicDroit.Opened

    '    Dim str As String = lb.Text



    'End Sub


    'Private Sub ClicDroit_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles ClicDroit.Opening

    '    'Bug en cours...

    '    Dim str As String = lb.Text

    '    ClicDroit.Items.Clear()

    '    ClicDroit.Items.Add(cMSDic.Keys(9), My.Resources.refresh)
    '    ClicDroit.Items.Item(ClicDroit.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

    '    ClicDroit.Items.Add(cMSDic.Keys(10), My.Resources.stop1)
    '    ClicDroit.Items.Item(ClicDroit.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

    '    'ADD NEW

    'End Sub


    Private Sub lb_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles lb.MouseMove
        Dim OverIndex As Integer = lb.IndexFromPoint(e.X, e.Y)
        If OverIndex <= ProgressBar1.Maximum Then
            lb.SelectedIndex = OverIndex
        End If
    End Sub

    Private Sub CheckUpdates_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Resize

        ProgressBar1.Width = ButtonExit.Location.X - ProgressBar1.Location.X - ProgressBar1.Margin.Left

        Logiciel.Width = Me.Width - (3 * Logiciel.Location.X)

        lb.Width = Logiciel.Size.Width
        lb.Height = ButtonExit.Location.Y - lb.Location.Y - Logiciel.Margin.Top


    End Sub

End Class
