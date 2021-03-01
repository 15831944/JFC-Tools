Option Strict On
Option Explicit On


Imports System.IO
Imports System.IO.Directory
Imports System.IO.DirectoryInfo

Imports VB = Microsoft.VisualBasic

Public Class Archivage

    Public USERS As String
    Public RepArchive As String
    Public RepRestaure As String
    Public StopProcess As Boolean = False
    Dim flag As Boolean
    Dim start, finish, totalTime As Double

    Private Sub ComboBoxArchives_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles ComboBoxArchives.SelectedIndexChanged

        ComboArchiveSelected()

    End Sub

   
    Private Sub ComboArchiveSelected()

        CheckBoxUfr02.Checked = False
        CheckBoxUfr03.Checked = False
        CheckBoxUfr09.Checked = False
        CheckBoxUfr10.Checked = False
        CheckBoxUfr14.Checked = False
        CheckBoxUfr15.Checked = False

        CheckBoxUfr02.Enabled = False
        CheckBoxUfr03.Enabled = False
        CheckBoxUfr09.Enabled = False
        CheckBoxUfr10.Enabled = False
        CheckBoxUfr14.Enabled = False
        CheckBoxUfr15.Enabled = False

        ListBoxEtudes.Items.Clear()

        Dim SelectedItem As String = ComboBoxArchives.Text 'CStr(ComboBoxArchives.SelectedItem).Trim

        If SelectedItem <> "" Then

            RepRestaure = USERS + "\" + CStr(ComboBoxArchives.SelectedItem)

            If IO.Directory.Exists(RepRestaure) Then

                CheckBoxUfr02.Enabled = IO.Directory.Exists(RepRestaure + "\UFR02")
                CheckBoxUfr03.Enabled = IO.Directory.Exists(RepRestaure + "\UFR03")
                CheckBoxUfr09.Enabled = IO.Directory.Exists(RepRestaure + "\UFR09")
                CheckBoxUfr10.Enabled = IO.Directory.Exists(RepRestaure + "\UFR10")
                CheckBoxUfr14.Enabled = IO.Directory.Exists(RepRestaure + "\UFR14")
                CheckBoxUfr15.Enabled = IO.Directory.Exists(RepRestaure + "\UFR15")

                If CheckBoxUfr02.Enabled And Not CheckBoxUfr03.Enabled And Not CheckBoxUfr09.Enabled And Not CheckBoxUfr10.Enabled And Not CheckBoxUfr14.Enabled And Not CheckBoxUfr15.Enabled Then
                    CheckBoxUfr02.Checked = True
                End If

                ButtonExecute.Enabled = True

                Rapide.Enabled = True

            End If

        End If
    End Sub

    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load

        USERS = Select_GetIniString("Localisation", "USER", Application.StartupPath & "\Ataaetud.ini")

        If USERS = "" Then
            MsgBox("Atelier Radio non trouvé.", vbCritical)
            End
        End If

        Dim CurrentPath = Application.StartupPath

        CurrentPath = Add(CurrentPath, BACKSLASH)

        ' Ajout un chemin au dossier si nécessaire
        If Mid(USERS, 2, 1) <> ":" And Mid(USERS, 2, 1) <> "\" Then USERS = CurrentPath & USERS

        LabelPath.Text = USERS

        RepArchive = USERS + "\Archives_" + CStr(Date.Now.Year) + StrFormat2Char(Date.Now.Month) + StrFormat2Char(Date.Now.Day) + StrFormat2Char(Date.Now.Hour) + StrFormat2Char(Date.Now.Minute) + StrFormat2Char(Date.Now.Second)

        ButtonExecute.Enabled = False
        Rapide.Enabled = False

        ModeArchivage()

    End Sub

    Private Sub ModeArchivage()

        LabelArchives.Visible = False
        ComboBoxArchives.Visible = False

        LabelStart.Visible = True
        LabelEnd.Visible = True
        DateTimeStart.Visible = True
        DateTimeEnd.Visible = True

        ListBoxEtudes.Items.Clear()

        Me.Text = "Atelier Radio - Archivage"

        DateTimeStart.Value = CDate("01/01/2000 0:00")

        DateTimeEnd.Value = CDate("01/01/2014 0:00")

        CheckBoxUfr02.Checked = False
        CheckBoxUfr03.Checked = False
        CheckBoxUfr09.Checked = False
        CheckBoxUfr10.Checked = False
        CheckBoxUfr14.Checked = False
        CheckBoxUfr15.Checked = False

        CheckBoxUfr02.Enabled = IO.Directory.Exists(USERS + "\UFR02")
        CheckBoxUfr03.Enabled = IO.Directory.Exists(USERS + "\UFR03")
        CheckBoxUfr09.Enabled = IO.Directory.Exists(USERS + "\UFR09")
        CheckBoxUfr10.Enabled = IO.Directory.Exists(USERS + "\UFR10")
        CheckBoxUfr14.Enabled = IO.Directory.Exists(USERS + "\UFR14")
        CheckBoxUfr15.Enabled = IO.Directory.Exists(USERS + "\UFR15")

        If CheckBoxUfr02.Enabled And Not CheckBoxUfr03.Enabled And Not CheckBoxUfr09.Enabled And Not CheckBoxUfr10.Enabled And Not CheckBoxUfr14.Enabled And Not CheckBoxUfr15.Enabled Then
            CheckBoxUfr02.Checked = True
        End If

    End Sub

    Private Sub ModeRestaure()

        LabelArchives.Visible = True
        ComboBoxArchives.Visible = True

        ButtonExecute.Enabled = False
        Rapide.Enabled = False

        LabelStart.Visible = False
        LabelEnd.Visible = False
        DateTimeStart.Visible = False
        DateTimeEnd.Visible = False

        Me.Text = "Atelier Radio - Restauration"

        Dim listrep() As String = IO.Directory.GetDirectories(USERS, "Archives_201???????????")

        ComboBoxArchives.Items.Clear()
        ComboBoxArchives.Text = ""
        ListBoxEtudes.Items.Clear()

        For Each rep As String In listrep

            If IO.Directory.GetFiles(rep, "*.*", IO.SearchOption.AllDirectories).Length > 0 Then
                ComboBoxArchives.Items.Add(rep.Replace(USERS + "\", ""))
            End If

        Next

        If ComboBoxArchives.Items.Count = 1 Then
            ComboBoxArchives.SelectedIndex = 0
        End If

        ComboArchiveSelected

    End Sub

    Private Function StrFormat2Char(ByVal Obj As Object) As String

        Return VB.Right("00" + CStr(Obj).Trim, 2)

    End Function

    Private Sub ButtonStart_Click(sender As System.Object, e As System.EventArgs) Handles ButtonStart.Click

        Me.Enabled = False

        ButtonStart.Enabled = False

        ListBoxEtudes.Items.Clear()

        start = Microsoft.VisualBasic.DateAndTime.Timer


        If Not CheckBoxUfr02.Checked And Not CheckBoxUfr03.Checked And Not CheckBoxUfr09.Checked And Not CheckBoxUfr10.Checked And Not CheckBoxUfr14.Checked And Not CheckBoxUfr15.Checked Then
            MsgBox("Auncun univers séléctionée.", MsgBoxStyle.Exclamation)

        Else 'If MsgBox("Etés-vous sur de vouloir archiver les etudes" + vbNewLine + "du " + CStr(DateTimeStart.Value) + " au " + CStr(DateTimeEnd.Value) + " ?", CType(MsgBoxStyle.Information + MsgBoxStyle.YesNo, MsgBoxStyle)) = MsgBoxResult.Yes Then

            'Directory.CreateDirectory(RepArchive)

            If CheckBoxUfr02.Checked Then
                If CheckUnivers("UFR02") = vbCancel Then GoTo fin
            End If

            If CheckBoxUfr03.Checked Then
                If CheckUnivers("UFR03") = vbCancel Then GoTo fin
            End If

            If CheckBoxUfr09.Checked Then
                If CheckUnivers("UFR09") = vbCancel Then GoTo fin
            End If

            If CheckBoxUfr10.Checked Then
                If CheckUnivers("UFR10") = vbCancel Then GoTo fin
            End If

            If CheckBoxUfr14.Checked Then
                If CheckUnivers("UFR14") = vbCancel Then GoTo fin
            End If

            If CheckBoxUfr15.Checked Then
                If CheckUnivers("UFR15") = vbCancel Then GoTo fin
            End If

fin:
            If ModeRestaurationToolStripMenuItem.Checked Then
                MsgBox(CStr(ListBoxEtudes.Items.Count) + " études sont prête à être restaurée dans: " + vbNewLine + vbNewLine + Chr(34) + USERS + Chr(34), MsgBoxStyle.Information, "Temps: " + CStr(Int(totalTime)).Trim + "secondes")
            Else
                MsgBox(CStr(ListBoxEtudes.Items.Count) + " études sont prête à être archivées dans :" + vbNewLine + vbNewLine + Chr(34) + RepArchive + Chr(34), MsgBoxStyle.Information, "Temps: " + CStr(Int(totalTime)).Trim + "secondes")
            End If

        End If

        ButtonExecute.Enabled = (ListBoxEtudes.Items.Count > 0) And Not ModeTestChargementToolStripMenuItem.Checked

        Rapide.Enabled = (ListBoxEtudes.Items.Count > 0) And Not ModeTestChargementToolStripMenuItem.Checked

        ButtonStart.Enabled = True

        Me.Enabled = True

    End Sub

    Private Function CheckUnivers(ByVal Univers As String) As Integer

        Dim rep As String = ""

        If ModeRestaurationToolStripMenuItem.Checked Then
            rep = RepRestaure
        Else
            rep = USERS
        End If

        PleaseWait.Show()

        Me.Hide()

        System.Windows.Forms.Application.DoEvents()

        Dim ListEtudes() As String = IO.Directory.GetFiles(rep + "\" + Univers, "ETU*.CSV", IO.SearchOption.AllDirectories)

        PleaseWait.Close()

        Me.Show()

        Me.TopMost = True

        Me.TopLevel = True

        System.Windows.Forms.Application.DoEvents()

        For Each Etude As String In ListEtudes
            If CheckEtude(Etude) = vbCancel Then
                Return vbCancel
            End If
        Next

        Return vbOK

    End Function


    Private Function CheckEtude(ByVal FileEtudeCsv As String) As Integer

        Dim svFile = ParsePath(FileEtudeCsv, FILENAME_ONLY)

        Dim NumEtude As String = ""

        If svFile.Length = 8 Then
            NumEtude = Mid(svFile, 4, 5)
        End If

        If NumEtude <> "" Then

            Dim InfoEtude() As String = GetNameOfEtude(FileEtudeCsv)

            Dim NameEtude As String = InfoEtude(0)

            If NameEtude <> vbNullString Then

                If ModeRestaurationToolStripMenuItem.Checked Then

                    If Not ModeTestChargementToolStripMenuItem.Checked Then

                        Dim ListFileEtude() As String = IO.Directory.GetFiles(ParsePath(FileEtudeCsv, PATH), "*" + NumEtude.Trim + ".*", IO.SearchOption.TopDirectoryOnly)

                        Dim eDate As Date = New Date

                        For Each file As String In ListFileEtude

                            Dim tFile As Date = IO.File.GetLastWriteTime(file)

                            If IsFileReadOnly(file) Then
                                If Not EclusFichierLectureSeuleToolStripMenuItem.Checked Then
                                    If MsgBox("L'étude " + Chr(34) + NameEtude + Chr(34) + " ne peut être restaurée," + vbNewLine + vbNewLine + "car le fichier suivant est protégé en écriture: " + vbNewLine + file + vbNewLine + vbNewLine + "Voulez-vous continuer ?", CType(MsgBoxStyle.Critical + MsgBoxStyle.YesNo, MsgBoxStyle)) = MsgBoxResult.Yes Then
                                        Return vbAbort
                                    Else
                                        Return vbCancel
                                    End If
                                Else
                                    Return vbAbort
                                End If
                            ElseIf DateTime.Compare(tFile, eDate) > 0 Then
                                eDate = tFile
                            End If
                        Next

                        Dim LastDateEtude As String = eDate.Date.Year.ToString + "-" + VB.Right("00" + eDate.Date.Month.ToString, 2) + "-" + VB.Right("00" + eDate.Date.Day.ToString, 2)

                        ListBoxEtudes.Items.Add(NameEtude + "|" + InfoEtude(1) + "|" + LastDateEtude + "|" + FileEtudeCsv.Replace(RepRestaure, "") + "|" + CStr(ListFileEtude.Length).Trim + " fichiers")

                    Else

                        ListBoxEtudes.Items.Add(NameEtude)

                    End If

                    totalTime = Microsoft.VisualBasic.DateAndTime.Timer - start

                    Me.Text = "Atelier Radio - Restauration - " + CStr(ListBoxEtudes.Items.Count) + " - " + CStr(Int(totalTime)).Trim + "s"

                    'System.Windows.Forms.Application.DoEvents()
                Else

                    Dim ListFileEtude() As String

                    If Not ModeTestChargementToolStripMenuItem.Checked Then
                        'ListFileEtude = {FileEtudeCsv}

                        ListFileEtude = IO.Directory.GetFiles(ParsePath(FileEtudeCsv, PATH), "*" + NumEtude.Trim + ".*", IO.SearchOption.TopDirectoryOnly)

                        Dim eDate As Date = New Date

                        For Each file As String In ListFileEtude

                            Dim tFile As Date = IO.File.GetLastWriteTime(file)
                            Dim tStart As Date = DateTimeStart.Value
                            Dim tEnd As Date = DateTimeEnd.Value


                            If DateTime.Compare(tFile, tStart) < 0 Or DateTime.Compare(tFile, tEnd) > 0 Then
                                Return vbAbort
                            ElseIf IsFileReadOnly(file) Then
                                If Not EclusFichierLectureSeuleToolStripMenuItem.Checked Then
                                    If MsgBox("L'étude " + Chr(34) + NameEtude.Trim + Chr(34) + " ne peut être archivée," + vbNewLine + vbNewLine + "car le fichier suivant est protégé en écriture: " + vbNewLine + file + vbNewLine + vbNewLine + "Voulez-vous continuer ?", CType(MsgBoxStyle.Critical + MsgBoxStyle.YesNo, MsgBoxStyle)) = MsgBoxResult.Yes Then
                                        Return vbAbort
                                    Else
                                        Return vbCancel
                                    End If
                                Else
                                    Return vbAbort
                                End If
                            ElseIf DateTime.Compare(tFile, eDate) > 0 Then
                                eDate = tFile
                            End If
                        Next

                        Dim LastDateEtude As String = eDate.Date.Year.ToString + "-" + VB.Right("00" + eDate.Date.Month.ToString, 2) + "-" + VB.Right("00" + eDate.Date.Day.ToString, 2)

                        ListBoxEtudes.Items.Add(NameEtude + "|" + InfoEtude(1) + "|" + LastDateEtude + "|" + FileEtudeCsv.Replace(USERS, "") + "|" + CStr(ListFileEtude.Length).Trim + " fichiers")

                    Else

                        ListBoxEtudes.Items.Add(NameEtude)

                    End If

                    'System.Windows.Forms.Application.DoEvents()

                    totalTime = Microsoft.VisualBasic.DateAndTime.Timer - start

                    Me.Text = "Atelier Radio - Archivage - " + CStr(ListBoxEtudes.Items.Count) + " - " + CStr(Int(totalTime)).Trim + "s"

                End If

                Else
                    Return vbAbort
                End If

        Else
            Return vbAbort
        End If

        Return vbOK

    End Function


    Private Function GetNameOfEtude(ByVal file As String) As String()

        Dim FileContenu() As String = IO.File.ReadAllLines(file, System.Text.Encoding.Default)

        If FileContenu.Length > 0 Then

            Dim a() As String = Split(FileContenu(0), ",")

            If a.Length > 2 Then
                Dim ReturnValue(1) As String
                ReturnValue(0) = VB.Left(a(1) + Space(32), 32).Replace(Chr(34), "")
                If a(4) = "1" Then
                    ReturnValue(1) = "ETUDE   "
                Else
                    ReturnValue(1) = "CAMPAGNE"
                End If
                Return ReturnValue
            ElseIf AjoutÉtudesÉrronéesToolStripMenuItem.Checked Then
                Return {("***** ERREUR DE LECTURE ******"), ("INCONNU ")}
            Else
                Return {(""), ("        ")}
            End If

        ElseIf AjoutÉtudesÉrronéesToolStripMenuItem.Checked Then
            Return {("***** ERREUR DE LECTURE ******"), ("INCONNU ")}
        Else
            Return {(""), ("        ")}
        End If

    End Function

    Private Sub ButtonQuitter_Click(sender As System.Object, e As System.EventArgs) Handles ButtonQuitter.Click
        If MsgBox("Etès-vous sur de vouloir quitter l'application ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo, MsgBoxStyle)) = MsgBoxResult.Yes Then
            StopProcess = True
            Me.Close()
        End If
    End Sub

   
    Private Sub ButtonExecute_Click(sender As System.Object, e As System.EventArgs) Handles ButtonExecute.Click

        'Me.Enabled = False

        ButtonExecute.Enabled = False

        Rapide.Enabled = False

        ButtonStart.Enabled = False

        Dim ListSelectedItems As ListBox.SelectedObjectCollection = ListBoxEtudes.SelectedItems

        Dim nNumEtude As Integer = ListSelectedItems.Count

        Do While ListSelectedItems.Count > 0

            Dim Items() As String = Split(CStr(ListSelectedItems.Item(0)), "|")

            Dim FileEtudeCsv As String = Items(3)

            Dim svFile = ParsePath(Items(3), FILENAME_ONLY)

            If ModeRestaurationToolStripMenuItem.Checked Then
                FileEtudeCsv = RepRestaure + FileEtudeCsv
            Else
                FileEtudeCsv = USERS + FileEtudeCsv
            End If

            Dim NumEtude As String = ""

            If svFile.Length = 8 Then
                NumEtude = Mid(svFile, 4, 5)
            End If

            If NumEtude <> "" Then

                Dim ListFileEtude() As String = IO.Directory.GetFiles(ParsePath(FileEtudeCsv, PATH), "*" + NumEtude.Trim + ".*", IO.SearchOption.TopDirectoryOnly)

                For Each file As String In ListFileEtude

                    Dim filemove As String
                    If ModeRestaurationToolStripMenuItem.Checked Then
                        filemove = file.Replace(RepRestaure, USERS)
                    Else
                        filemove = file.Replace(USERS, RepArchive)
                    End If

                    Directory.CreateDirectory(ParsePath(filemove, PATH))

                    If StopProcess Then
                        End
                    End If

                    IO.File.Move(file, filemove)

                    Dim DirectoryEmpty As String = ParsePath(file, PATH)

                    If IO.Directory.GetFiles(DirectoryEmpty, "*.*", IO.SearchOption.AllDirectories).Length = 0 Then
                        IO.Directory.Delete(DirectoryEmpty, True)
                    End If

                Next

            End If

            ListBoxEtudes.Items.RemoveAt(ListBoxEtudes.Items.IndexOf(ListSelectedItems.Item(0)))

            If ModeRestaurationToolStripMenuItem.Checked Then
                Me.Text = "Atelier Radio - Restauration - " + CStr(ListBoxEtudes.Items.Count)
            Else
                Me.Text = "Atelier Radio - Archivage - " + CStr(ListBoxEtudes.Items.Count)
            End If


            System.Windows.Forms.Application.DoEvents()
        Loop

        If ModeRestaurationToolStripMenuItem.Checked Then
            MsgBox(CStr(nNumEtude) + " études ont été restorées dans le dossier suivant:" + vbNewLine + vbNewLine + Chr(34) + USERS + Chr(34), MsgBoxStyle.Information)
        Else
            MsgBox(CStr(nNumEtude) + " études ont été archivées dans le dossier suivant:" + vbNewLine + vbNewLine + Chr(34) + RepArchive + Chr(34), MsgBoxStyle.Information)
        End If

        If (ListBoxEtudes.Items.Count = 0) Then
            ModeRestaure()
        End If

        ButtonExecute.Enabled = True

        Rapide.Enabled = True

        ButtonStart.Enabled = True

        'Me.Enabled = True

    End Sub

    Private Sub ListBoxEtudes_KeyDown(sender As Object, e As System.Windows.Forms.KeyEventArgs) Handles ListBoxEtudes.KeyDown

        If e.KeyCode = Keys.A AndAlso e.Control = True Then

            Dim i As Integer
            For i = 0 To Me.ListBoxEtudes.Items.Count - 1
                Me.ListBoxEtudes.SetSelected(i, True)
            Next i

            e.Handled = True

            e.SuppressKeyPress = True

        End If

    End Sub

    Private Sub ModeRestaurationToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles ModeRestaurationToolStripMenuItem.Click

        If ModeRestaurationToolStripMenuItem.Checked Then
            ModeRestaurationToolStripMenuItem.CheckState = CheckState.Unchecked
            'ModeRestaurationToolStripMenuItem.Text = "Mode restauration"
            ModeArchivage()
        Else
            ModeRestaurationToolStripMenuItem.CheckState = CheckState.Checked
            'ModeRestaurationToolStripMenuItem.Text = "Mode archivage"
            ModeRestaure()
        End If

        ButtonExecute.Enabled = False

        Rapide.Enabled = False

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub TriCroissantToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles TriCroissantToolStripMenuItem.Click

        If TriCroissantToolStripMenuItem.Checked Then
            TriCroissantToolStripMenuItem.CheckState = CheckState.Unchecked
            ListBoxEtudes.Sorted = False
        Else
            TriCroissantToolStripMenuItem.CheckState = CheckState.Checked
            ListBoxEtudes.Sorted = True
        End If

    End Sub

    Private Sub AjoutÉtudesÉrronéesToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles AjoutÉtudesÉrronéesToolStripMenuItem.Click
        If AjoutÉtudesÉrronéesToolStripMenuItem.Checked Then
            AjoutÉtudesÉrronéesToolStripMenuItem.CheckState = CheckState.Unchecked
        Else
            AjoutÉtudesÉrronéesToolStripMenuItem.CheckState = CheckState.Checked
        End If
    End Sub

   
    Private Sub EclusFichierLectureSeuleToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles EclusFichierLectureSeuleToolStripMenuItem.Click
        If EclusFichierLectureSeuleToolStripMenuItem.Checked Then
            EclusFichierLectureSeuleToolStripMenuItem.CheckState = CheckState.Unchecked
        Else
            EclusFichierLectureSeuleToolStripMenuItem.CheckState = CheckState.Checked
        End If
    End Sub

    Private Sub AProposToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles AProposToolStripMenuItem.Click
        AboutBoxArchivage.ShowDialog()
    End Sub

    Private Sub LabelPath_Click(sender As System.Object, e As System.EventArgs) Handles LabelPath.Click
        Dim arg As String = "/select," + USERS + "\"
        System.Diagnostics.Process.Start("Explorer.exe", arg)
    End Sub


    Private Function GetPlanOfEtude(ByVal file As String) As String()

        If IO.File.Exists(file) Then

            Dim FileContenu() As String = IO.File.ReadAllLines(file, System.Text.Encoding.Default)

            Dim ReturnValue(FileContenu.Length) As String

            Dim index As Integer = 0

            If FileContenu.Length > 0 Then
                For i = 0 To FileContenu.Length - 1
                    Dim a() As String = Split(FileContenu(i), ",")
                    If a.Length > 2 Then
                        If a(1) <> Chr(34) + Chr(34) Then
                            Dim b = a(1).Replace(Chr(34), "")
                            If Not ReturnValue.Contains(b) Then
                                ReturnValue(index) = b
                                index = index + 1
                            End If
                        End If
                    End If
                Next
                ReDim Preserve ReturnValue(index - 1)
                Return ReturnValue
            Else
                Return {""}
            End If

        Else
            Return {""}
        End If

    End Function

    Private Sub ListBoxEtudes_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles ListBoxEtudes.SelectedIndexChanged

        If Not ModeTestChargementToolStripMenuItem.Checked Then
            SyncLock Me

                ListBoxPlan.Items.Clear()

                If ListBoxEtudes.SelectedItems.Count = 1 Then

                    Dim SelectedItem = ListBoxEtudes.SelectedItem
                    Dim Items() As String = Split(CStr(SelectedItem), "|")

                    Dim svFile = ParsePath(Items(3), FILENAME_ONLY)
                    Dim NumEtude As String = ""
                    If svFile.Length = 8 Then
                        NumEtude = Mid(svFile, 4, 5)
                    End If

                    Dim FilePlanCsv As String = Items(3)
                    Dim Title As String = ""
                    If ModeRestaurationToolStripMenuItem.Checked Then
                        FilePlanCsv = RepRestaure + FilePlanCsv.Replace("ETU" + NumEtude, "PLN" + NumEtude)
                    Else
                        FilePlanCsv = USERS + FilePlanCsv.Replace("ETU" + NumEtude, "PLN" + NumEtude)
                    End If

                    If NumEtude <> "" Then
                        Dim ListPlan() As String = GetPlanOfEtude(FilePlanCsv)
                        If ListPlan.Length = 1 And ListPlan(0) = "" Then
                            ListBoxPlan.Items.Add("Aucune information.")
                        ElseIf ListPlan.Length > 0 Then
                            ListBoxPlan.Items.AddRange(ListPlan)
                        End If
                    End If

                End If

            End SyncLock
        End If

    End Sub

    Private Sub ModeTestChargementToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles ModeTestChargementToolStripMenuItem.Click
        If ModeTestChargementToolStripMenuItem.Checked Then
            ModeTestChargementToolStripMenuItem.CheckState = CheckState.Unchecked
            DateTimeStart.Enabled = True
            DateTimeEnd.Enabled = True
        Else
            ModeTestChargementToolStripMenuItem.CheckState = CheckState.Checked
        End If

        DateTimeStart.Enabled = Not ModeTestChargementToolStripMenuItem.Checked
        DateTimeEnd.Enabled = Not ModeTestChargementToolStripMenuItem.Checked


    End Sub

    Private Sub Rapide_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Rapide.Click

        'Me.Enabled = False

        ButtonExecute.Enabled = False

        Rapide.Enabled = False

        ButtonStart.Enabled = False

        Dim nNumEtude As Integer = ListBoxEtudes.Items.Count

        For i = 0 To nNumEtude - 1

            Dim Items() As String = Split(CStr(ListBoxEtudes.Items.Item(i)), "|")

            Dim FileEtudeCsv As String = Items(3)

            Dim svFile = ParsePath(Items(3), FILENAME_ONLY)

            FileEtudeCsv = USERS + FileEtudeCsv

            Dim NumEtude As String = ""

            If svFile.Length = 8 Then
                NumEtude = Mid(svFile, 4, 5)
            End If

            If NumEtude <> "" Then

                Dim ListFileEtude() As String = IO.Directory.GetFiles(ParsePath(FileEtudeCsv, PATH), "*" + NumEtude.Trim + ".*", IO.SearchOption.TopDirectoryOnly)

                For Each file As String In ListFileEtude

                    Dim filemove As String
                    filemove = file.Replace(USERS, RepArchive)

                    Directory.CreateDirectory(ParsePath(filemove, PATH))

                    If StopProcess Then
                        End
                    End If

                    IO.File.Move(file, filemove)

                    Dim DirectoryEmpty As String = ParsePath(file, PATH)

                    If IO.Directory.GetFiles(DirectoryEmpty, "*.*", IO.SearchOption.AllDirectories).Length = 0 Then
                        IO.Directory.Delete(DirectoryEmpty, True)
                    End If

                Next

            End If

            Me.Text = "Atelier Radio - Archivage - " + CStr(i) + "/" + CStr(nNumEtude)

            System.Windows.Forms.Application.DoEvents()

        Next

        MsgBox(CStr(nNumEtude) + " études ont été archivées dans le dossier suivant:" + vbNewLine + vbNewLine + Chr(34) + RepArchive + Chr(34), MsgBoxStyle.Information)

        ButtonExecute.Enabled = True

        Rapide.Enabled = True

        ButtonStart.Enabled = True

        'Me.Enabled = True

    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)

    End Sub
End Class
