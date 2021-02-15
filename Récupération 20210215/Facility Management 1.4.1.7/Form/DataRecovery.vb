Option Strict On
Option Explicit On

Public Class DataRecovery

    'Dim CheckedNodes As List(Of TreeNode)
    ReadOnly seprator As String = " - "
    Private mPath As String
    Private mDestination As String
    Public LaunchDirectClass As Boolean
    Public NameOfWave As String
    Public IsNotLockedTreeView As Boolean = True
    Public ZipPassword As String = ""
    Private Sub Init()
        mPath = Select_GetIniString(FormBox.MenuSoft, "Source", FormBox.FacilityManagementIni).Trim
        mDestination = Select_GetIniString(FormBox.MenuSoft, "Destination", FormBox.FacilityManagementIni).Trim
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        Init()

        Me.Opacity = FormBox.nvOpacity

        'Me.CheckedNodes = New List(Of TreeNode)

        System.Windows.Forms.Application.DoEvents()

        ReadSources()

        ToolStripStatusLabel2.Text = ""
        ToolStripProgressBar2.Value = 0

    End Sub

    Private Sub ReadSources()

        ComboBox1.Items.Clear()

        ComboBox1.ResetText()

        TreeViewList.Nodes.Clear()
        TreeViewSelected.Nodes.Clear()

        Dim SourcesNames() As String = IO.Directory.GetDirectories(mPath)

        For i As Integer = 0 To SourcesNames.Length - 1

            Dim mSources As String = Replace(SourcesNames(i), mPath, "")

            Dim SourceName As String = ReadSourceName(mPath & mSources & "\", mSources)

            If SourceName <> "" Then

                Dim mNameSources As String = mSources & seprator & ReadSourceName(mPath & mSources & "\", mSources)

                ComboBox1.Items.Add(mNameSources)

            End If

        Next

        If ComboBox1.Items.Count = 1 Then
            ComboBox1.SelectedIndex = 0
        End If

    End Sub

    Private Function AddNodes(ByVal node As TreeView, ByVal Libelle As String) As Integer

        Dim Noeud As String
        'If Libelle.Contains("CIBLE") Then
        '    Noeud = "CIBLE+"
        If Libelle.Contains("2007") Then
            Noeud = "2007"
        ElseIf Libelle.Contains("2008") Then
            Noeud = "2008"
        ElseIf Libelle.Contains("2009") Then
            Noeud = "2009"
        ElseIf Libelle.Contains("2010") Then
            Noeud = "2010"
        ElseIf Libelle.Contains("2011") Then
            Noeud = "2011"
        ElseIf Libelle.Contains("2012") Then
            Noeud = "2012"
        ElseIf Libelle.Contains("2013") Then
            Noeud = "2013"
        ElseIf Libelle.Contains("2014") Then
            Noeud = "2014"
        ElseIf Libelle.Contains("2015") Then
            Noeud = "2015"
        ElseIf Libelle.Contains("2016") Then
            Noeud = "2016"
        ElseIf Libelle.Contains("2017") Then
            Noeud = "2017"
        ElseIf Libelle.Contains("2018") Then
            Noeud = "2018"
        ElseIf Libelle.Contains("2019") Then
            Noeud = "2019"
        ElseIf Libelle.Contains("2020") Then
            Noeud = "2020"
        Else
            Noeud = "Year missing"
        End If

        If Not node.Nodes.ContainsKey(Noeud) Then
            node.Nodes.Add(Noeud, Noeud)
        End If

        Dim IndexNoeud As Integer = node.Nodes.IndexOfKey(Noeud)
        If Not node.Nodes(IndexNoeud).Nodes.ContainsKey(Libelle) Then
            node.Nodes(IndexNoeud).Nodes.Add(Libelle, Libelle)
        End If

        Return IndexNoeud

    End Function

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged

        SelectedSources

    End Sub

    Private Sub SelectedSources()

        ButtonCreating.Enabled = False
        ButtonZip.Enabled = False
        ButtonUnZip.Enabled = False

        Dim mNameVague As String = RecFirstName(CStr(ComboBox1.SelectedItem))
        Dim mPathVagues As String = mPath & mNameVague & "\"

        Dim VaguesNames() As String = IO.Directory.GetDirectories(mPathVagues, "T?????.")

        'Check Data
        If IO.Directory.Exists(mDestination) Then
            Dim CheckData() As String = IO.Directory.GetDirectories(mDestination)
            If CheckData.Length + CInt(CheckData.Contains(mDestination + mNameVague)) > 0 Then
                If MsgBox("Would you erase data from other countries", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Clear Data") = MsgBoxResult.Yes Then
                    For Each z In CheckData
                        If Not CheckData.Contains(mDestination + mNameVague) Then
                            IO.Directory.Delete(z, True)
                        End If
                    Next
                End If
            End If
        Else
            IO.Directory.CreateDirectory(mDestination)
        End If

        ReDim Preserve VaguesNames(VaguesNames.Length - 1)

        TreeViewList.Nodes.Clear()
        TreeViewSelected.Nodes.Clear()

        TreeViewList.Sort()
        TreeViewSelected.Sort()

        For i As Integer = 0 To VaguesNames.Length - 1
            Dim mVague As String = Replace(VaguesNames(i), mPathVagues, "")
            Dim NameOfVague As String = ReadVagueName(VaguesNames(i), mVague)

            If IO.Directory.Exists(mDestination & mNameVague & "\" & mVague) Then
                AddNodes(TreeViewSelected, mVague & seprator & NameOfVague)
            Else
                AddNodes(TreeViewList, mVague & seprator & NameOfVague)
            End If
        Next

        ButtonCreating.Enabled = True
        ButtonZip.Enabled = True
        ButtonUnZip.Enabled = True

    End Sub


    Private Function ReadSourceName(ByVal svPath As String, ByVal svFiles As String) As String

        Dim nvCount As Integer = 28
        Dim svData As String = ""
        Dim mName() As String = IO.Directory.GetFiles(svPath, svFiles & "*.801", IO.SearchOption.TopDirectoryOnly)

        If mName.Length > 0 Then

            Dim ByteData() As Byte = IO.File.ReadAllBytes(mName(0))

            While ByteData(nvCount) <> 0
                svData += Chr(ByteData(nvCount))
                nvCount += 1
            End While

        End If

        ReadSourceName = svData


    End Function

    Private Function ReadVagueName(ByVal svPath As String, ByVal svFiles As String) As String

        Dim nvCount As Integer = 32
        Dim svData As String = ""
        Dim mName() As String = IO.Directory.GetFiles(svPath, svFiles & "*.801", IO.SearchOption.TopDirectoryOnly)

        Dim ByteData() As Byte = IO.File.ReadAllBytes(mName(0))

        While ByteData(nvCount) <> 0
            svData += Chr(ByteData(nvCount))
            nvCount += 1
        End While

        ReadVagueName = svData

    End Function

    Private Sub ButtonExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonExit.Click
        Me.Close()
    End Sub

    Private Sub ButtonCreating_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCreating.Click

        ButtonCreating.Enabled = False
        ButtonZip.Enabled = False
        ButtonUnZip.Enabled = False

        ToolStripProgressBar2.Maximum = CountFiles()
        ToolStripProgressBar2.Value = 0

        NameOfWave = ""

        Dim mSource As String = RecFirstName(ComboBox1.SelectedItem.ToString)
        Dim mFiles As String = mSource & "*.*"
        Dim mRepSrc As String = mPath & mSource & "\"
        Dim mRepDest As String = mDestination & mSource & "\"
        Dim bSource As Boolean = False

        For Each u As TreeNode In TreeViewSelected.Nodes
            For Each x As TreeNode In u.Nodes
                Dim mVague As String = RecFirstName(x.Name)
                NameOfWave = NameOfWave + RecLastName(x.Name) + vbNewLine
                CopyFiles(mRepSrc & mVague, mRepDest & mVague)
                If Not bSource Then bSource = True
            Next
        Next

        For Each u As TreeNode In TreeViewList.Nodes
            For Each x As TreeNode In u.Nodes
                Dim mVague As String = RecFirstName(x.Name)
                If IO.Directory.Exists(mRepDest & mVague) Then
                    IO.Directory.Delete(mRepDest & mVague, True)
                End If
            Next
        Next

        If bSource Then

            If MsgBox("Would you copy data from sources?" + Chr(13) & Chr(10) + Chr(13) & Chr(10) + "Ex: The 4 files of sources (001, 002, 003, 801)", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Sources Data") = MsgBoxResult.Yes Then
                CopyFiles(mRepSrc, mRepDest, mFiles)
            End If

        ElseIf IO.Directory.Exists(mRepDest) Then
            IO.Directory.Delete(mRepDest, True)
            'DeleteAllFiles(mRepDest, mFiles)
        End If

        MsgBox("Upgrade Completed.", MsgBoxStyle.Information)

        ToolStripStatusLabel2.Text = ""
        ToolStripProgressBar2.Value = 0
        ButtonCreating.Enabled = True
        ButtonZip.Enabled = True
        ButtonUnZip.Enabled = True


    End Sub

    Private Function RecFirstName(ByVal sValue As String) As String

        Dim nPos As Integer = InStr(sValue, seprator, CompareMethod.Text)

        If nPos > 0 Then
            RecFirstName = Mid(sValue, 1, nPos - 1)
        Else
            RecFirstName = sValue
        End If


    End Function

    Private Function RecLastName(ByVal sValue As String) As String

        Dim nPos As Integer = InStr(sValue, seprator, CompareMethod.Text)

        If nPos > 0 Then
            nPos += 3
            RecLastName = Mid(sValue, nPos, Len(sValue) - nPos + 1)
        Else
            RecLastName = sValue
        End If


    End Function

    Private Sub CopyFiles(ByVal mSrc As String, ByVal mDest As String, Optional ByVal SearchFile As String = "*.*", Optional ByVal Directory As System.IO.SearchOption = IO.SearchOption.TopDirectoryOnly)

        Dim ListFiles() As String = IO.Directory.GetFiles(mSrc, SearchFile, Directory)

        Dim mSrcFile As String, mDestFile As String, mDestDirectory As String

        If Strings.Right(mSrc, 1) <> "\" Then mSrc &= "\"

        If Strings.Right(mDest, 1) <> "\" Then mDest &= "\"

        For y As Integer = 0 To ListFiles.Length - 1
            mSrcFile = ListFiles(y)
            mDestFile = mDest & Replace(mSrcFile, mSrc, "")
            mDestDirectory = GetDirectory(mDestFile)
            If Not IO.Directory.Exists(mDestDirectory) Then
                IO.Directory.CreateDirectory(mDestDirectory)
            End If

            Dim nPosLastSlach As Integer = InStrRev(mDestFile, "\")
            ToolStripStatusLabel2.Text = Mid(mDestFile, nPosLastSlach + 1, Len(mDestFile) - nPosLastSlach)

            ToolStripProgressBar2.Value = ToolStripProgressBar2.Value + 1

            System.Windows.Forms.Application.DoEvents()
            IO.File.Copy(mSrcFile, mDestFile, True)
        Next


    End Sub

    Private Function GetDirectory(ByVal mDirectory As String) As String

        Dim nPos As Integer = InStrRev(mDirectory, "\")
        If nPos > 0 Then
            GetDirectory = Mid(mDirectory, 1, nPos - 1)
        Else
            GetDirectory = mDirectory
        End If

    End Function

    Private Function CountFiles() As Integer

        Dim mSource As String = RecFirstName(ComboBox1.SelectedItem.ToString)
        Dim mFiles As String = mSource & "*.*"
        Dim mRepSrc As String = mPath & mSource & "\"
        Dim mRepDest As String = mDestination & mSource & "\"

        Dim ListFilesSource() As String = IO.Directory.GetFiles(mRepSrc, mFiles, IO.SearchOption.TopDirectoryOnly)

        CountFiles = ListFilesSource.Length

        For Each u As TreeNode In TreeViewSelected.Nodes
            For Each x As TreeNode In u.Nodes

                Dim mVague As String = RecFirstName(x.Name)

                Dim ListFilesVague() As String = IO.Directory.GetFiles(mRepSrc & mVague, "*.*", IO.SearchOption.TopDirectoryOnly)

                CountFiles += ListFilesVague.Length

            Next
        Next

    End Function

    Private Sub TreeViewList_NodeMouseDoubleClick(sender As Object, e As System.Windows.Forms.TreeNodeMouseClickEventArgs) Handles TreeViewList.NodeMouseDoubleClick

        If e.Node.Level = 0 Then
            For Each x As TreeNode In e.Node.Nodes
                AddNodes(TreeViewSelected, x.Name)
            Next
        Else
            TreeViewSelected.Nodes(AddNodes(TreeViewSelected, e.Node.Name)).Expand()

        End If

        If e.Node.Level = 0 Then
            e.Node.Remove()
        ElseIf e.Node.Parent.Nodes.Count = 1 Then
            e.Node.Parent.Remove()
        Else
            e.Node.Remove()
        End If

    End Sub

    Private Sub TreeViewSelected_NodeMouseDoubleClick(sender As Object, e As System.Windows.Forms.TreeNodeMouseClickEventArgs) Handles TreeViewSelected.NodeMouseDoubleClick

        If e.Node.Level = 0 Then
            For Each x As TreeNode In e.Node.Nodes
                AddNodes(TreeViewList, x.Name)
            Next
        Else
            AddNodes(TreeViewList, e.Node.Name)
        End If

        If e.Node.Level = 0 Then
            e.Node.Remove()
        ElseIf e.Node.Parent.Nodes.Count = 1 Then
            e.Node.Parent.Remove()
        Else
            e.Node.Remove()
        End If

    End Sub

    Private Sub ButtonZip_Click(sender As System.Object, e As System.EventArgs) Handles ButtonZip.Click

        ButtonZip.Enabled = False
        ButtonCreating.Enabled = False
        ButtonUnZip.Enabled = False

        'Dim szZipFile As String = System.Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\Update.zip"

        SaveFileDialog1.InitialDirectory = System.Environment.GetFolderPath(Environment.SpecialFolder.Desktop)
        SaveFileDialog1.Filter = "zip files (*.zip)|*.zip|All files (*.*)|*.*"
        SaveFileDialog1.FilterIndex = 1
        SaveFileDialog1.RestoreDirectory = True
        SaveFileDialog1.FileName = "Update.zip"
        SaveFileDialog1.ShowDialog()
        Dim szZipFile As String = SaveFileDialog1.FileName

        ToolStripProgressBar2.Maximum = CountFiles()
        ToolStripProgressBar2.Value = 0

        Dim mSource As String = RecFirstName(ComboBox1.SelectedItem.ToString)
        Dim mFiles As String = mSource & "*.*"
        Dim mRepSrc As String = mPath & mSource & "\"
        Dim mRepDest As String = mDestination & mSource & "\"
        Dim bSource As Boolean = False

        For Each u As TreeNode In TreeViewSelected.Nodes
            For Each x As TreeNode In u.Nodes
                Dim mVague As String = RecFirstName(x.Name)
                NameOfWave = RecLastName(x.Name) + vbNewLine
                CopyFiles(mRepSrc & mVague, mRepDest & mVague)
                If Not bSource Then bSource = True
            Next
        Next

        For Each u As TreeNode In TreeViewList.Nodes
            For Each x As TreeNode In u.Nodes
                Dim mVague As String = RecFirstName(x.Name)
                If IO.Directory.Exists(mRepDest & mVague) Then
                    IO.Directory.Delete(mRepDest & mVague, True)
                End If
            Next
        Next

        If bSource Then
            CopyFiles(mRepSrc, mRepDest, mFiles)
        ElseIf IO.Directory.Exists(mRepDest) Then
            IO.Directory.Delete(mRepDest, True)
            'DeleteAllFiles(mRepDest, mFiles)
        End If

        GenerateZip(mDestination, szZipFile)

        MsgBox("Zip file creation finished." + vbNewLine + "Zip location: " + Chr(34) + szZipFile + Chr(34), MsgBoxStyle.Information)

        'Try
        '    IO.Directory.Delete(mRepSrc, True)
        'Catch ex As Exception

        'End Try

        ToolStripStatusLabel2.Text = ""
        ToolStripProgressBar2.Value = 0

        ButtonZip.Enabled = True
        ButtonCreating.Enabled = True
        ButtonUnZip.Enabled = True

    End Sub

    Public Sub ExtractZip(ByVal zipStream As String, ByVal outputFolder As String, Optional ByVal RootFolder As String = "")

        If Not IO.Directory.Exists(outputFolder) Then
            IO.Directory.CreateDirectory(outputFolder)
        End If

        Dim zipFile As ZipFile = New ZipFile(zipStream) With {
            .Password = ZipPassword
        }

        'Dim a As Integer = zipFile.FindEntry("Update.inf", False)

        Try

            For i = 0 To zipFile.Count - 1

                Dim entry As ZipEntry = zipFile(CInt(i))

                Dim fileName As String = entry.Name


                'Evite le double dossier
                If RootFolder <> Nothing Then
                    If InStr(fileName, RootFolder, CompareMethod.Text) = 1 Then
                        fileName = fileName.Replace(RootFolder + "/", "")
                    End If
                End If


                If (entry.IsDirectory) Then

                    Dim path As String = IO.Path.Combine(outputFolder, fileName)

                    If Not IO.Directory.Exists(path) Then
                        IO.Directory.CreateDirectory(path)
                    End If

                ElseIf (entry.IsFile) Then

                    Dim path As String = IO.Path.Combine(outputFolder, fileName)

                    Dim dir As String = IO.Path.GetDirectoryName(path)
                    If Not IO.Directory.Exists(dir) Then
                        IO.Directory.CreateDirectory(dir)
                    End If

                    Dim Input As IO.Stream = zipFile.GetInputStream(entry)

                    WriteToFile(Input, path)

                    Input.Close()

                    IO.File.SetCreationTime(path, entry.DateTime)
                    IO.File.SetLastWriteTime(path, entry.DateTime)
                    IO.File.SetLastAccessTime(path, entry.DateTime)

                End If

            Next
        Finally
            zipFile.Close()
        End Try

    End Sub

    Private Sub WriteToFile(ByVal input As IO.Stream, ByVal filePath As String)

        If (IO.File.Exists(filePath)) Then
            IO.File.Delete(filePath)
        End If

        Dim output As IO.FileStream = New IO.FileStream(filePath, IO.FileMode.OpenOrCreate)

        Dim buffer As Byte() = New Byte(8192) {}

        Dim size As Integer = input.Read(buffer, 0, buffer.Length)

        While (size <> 0)
            output.Write(buffer, 0, size)
            size = input.Read(buffer, 0, buffer.Length)
            System.Windows.Forms.Application.DoEvents()
        End While

        output.Close()

    End Sub

    Public Sub GenerateZip(ByVal inputPath As String, ByVal targetLocation As String)


        Dim files = IO.Directory.GetFiles(inputPath, "*.*", IO.SearchOption.AllDirectories)

        ToolStripProgressBar2.Maximum = files.Count
        ToolStripProgressBar2.Value = 0
        ToolStripStatusLabel2.Text = ""
        System.Windows.Forms.Application.DoEvents()

        Dim fileMap As List(Of String()) = New List(Of String())

        inputPath = IO.Directory.GetParent(inputPath).FullName

        Dim len As Integer = inputPath.Length + 1

        For Each file In files

            fileMap.Add(New String() {file, file.Substring(len)})

        Next

        If (IO.File.Exists(targetLocation)) Then IO.File.Delete(targetLocation)

        Dim stream As IO.FileStream = New IO.FileStream(targetLocation, IO.FileMode.CreateNew)

        Dim zipStream As ZipOutputStream = New ZipOutputStream(stream) With {
            .Password = ZipPassword
        }

        For Each filePeer In fileMap

            Dim entryName As String = filePeer(1)
            Dim fileName As String = filePeer(0)

            Dim nPosLastSlach As Integer = InStrRev(fileName, "\")
            ToolStripStatusLabel2.Text = "Zip: " + Mid(fileName, nPosLastSlach + 1, fileName.Length - nPosLastSlach)

            System.Windows.Forms.Application.DoEvents()

            Dim buf = IO.File.ReadAllBytes(fileName)

            Dim entry As ZipEntry = New ZipEntry(ZipEntry.CleanName(entryName)) With {
                .IsUnicodeText = True
            }

            zipStream.PutNextEntry(entry)

            zipStream.Write(buf, 0, buf.Length)

            zipStream.CloseEntry()

            ToolStripProgressBar2.Value = ToolStripProgressBar2.Value + 1

            System.Windows.Forms.Application.DoEvents()

        Next

        zipStream.Finish()
        zipStream.Close()

    End Sub

    Public Sub TreeViewSelected_ItemDrag(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.ItemDragEventArgs) _
        Handles TreeViewSelected.ItemDrag

        'Set the drag node and initiate the DragDrop 
        DoDragDrop(e.Item, DragDropEffects.Move)

    End Sub

    Public Sub TreeViewSelected_DragEnter(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewSelected.DragEnter

        'See if there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
            True) Then
            'TreeNode found allow move effect
            e.Effect = DragDropEffects.Move
        Else
            'No TreeNode found, prevent move
            e.Effect = DragDropEffects.None
        End If

    End Sub

    'Public Sub TreeViewSelected_DragOver(ByVal sender As System.Object, _
    '   ByVal e As DragEventArgs) _
    '   Handles TreeViewSelected.DragOver

    '    'Check that there is a TreeNode being dragged 
    '    If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
    '           True) = False Then Exit Sub

    '    'Get the TreeView raising the event (incase multiple on form)
    '    Dim selectedTreeview As TreeView = CType(sender, TreeView)

    '    'As the mouse moves over nodes, provide feedback to 
    '    'the user by highlighting the node that is the 
    '    'current drop target
    '    Dim pt As Point = _
    '        CType(sender, TreeView).PointToClient(New Point(e.X, e.Y))
    '    Dim targetNode As TreeNode = selectedTreeview.GetNodeAt(pt)

    '    'See if the targetNode is currently selected, 
    '    'if so no need to validate again
    '    If Not (selectedTreeview.SelectedNode Is targetNode) Then
    '        'Select the    node currently under the cursor
    '        selectedTreeview.SelectedNode = targetNode

    '        'Check that the selected node is not the dropNode and
    '        'also that it is not a child of the dropNode and 
    '        'therefore an invalid target
    '        Dim dropNode As TreeNode = _
    '            CType(e.Data.GetData("System.Windows.Forms.TreeNode"),  _
    '            TreeNode)

    '        Do Until targetNode Is Nothing
    '            If targetNode Is dropNode Then
    '                e.Effect = DragDropEffects.None
    '                Exit Sub
    '            End If
    '            targetNode = targetNode.Parent
    '        Loop
    '    'End If

    '    'Currently selected node is a suitable target
    '    e.Effect = DragDropEffects.Move

    '    'End If

    'End Sub

    Public Sub TreeViewSelected_DragDrop(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewSelected.DragDrop

        'Check that there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
              True) = False Then Exit Sub

        'Get the TreeView raising the event (incase multiple on form)
        Dim selectedTreeview As TreeView = CType(sender, TreeView)

        'Get the TreeNode being dragged
        Dim dropNode As TreeNode = _
              CType(e.Data.GetData("System.Windows.Forms.TreeNode"),  _
              TreeNode)

        'The target node should be selected from the DragOver event
        Dim targetNode As TreeNode = selectedTreeview.SelectedNode

        'Remove the drop node from its current location
        'dropNode.Remove()

        'If there is no targetNode add dropNode to the bottom of
        'the TreeView root nodes, otherwise add it to the end of
        'the dropNode child nodes

        'Modif Olive (Debut)
        If dropNode.Level = 0 Then
            For Each x As TreeNode In dropNode.Nodes
                AddNodes(selectedTreeview, x.Name)
            Next
        Else
            AddNodes(selectedTreeview, dropNode.Name)
        End If

        If dropNode.Level = 0 Then
            dropNode.Remove()
        ElseIf dropNode.Parent.Nodes.Count = 1 Then
            dropNode.Parent.Remove()
        Else
            dropNode.Remove()
        End If
        'Modif Olive (Fin)

        'If targetNode Is Nothing Then
        '    selectedTreeview.Nodes.Add(dropNode)
        'Else
        '    targetNode.Nodes.Add(dropNode)
        'End If

        'Ensure the newley created node is visible to
        'the user and select it
        dropNode.EnsureVisible()
        selectedTreeview.SelectedNode = dropNode

    End Sub


    Public Sub TreeViewList_ItemDrag(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.ItemDragEventArgs) _
        Handles TreeViewList.ItemDrag

        'Set the drag node and initiate the DragDrop 
        DoDragDrop(e.Item, DragDropEffects.Move)

    End Sub

    Public Sub TreeViewList_DragEnter(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewList.DragEnter

        'See if there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
            True) Then
            'TreeNode found allow move effect
            e.Effect = DragDropEffects.Move
        Else
            'No TreeNode found, prevent move
            e.Effect = DragDropEffects.None
        End If

    End Sub

    'Public Sub TreeViewList_DragOver(ByVal sender As System.Object, _
    '   ByVal e As DragEventArgs) _
    '   Handles TreeViewList.DragOver

    '    'Check that there is a TreeNode being dragged 
    '    If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
    '           True) = False Then Exit Sub

    '    'Get the TreeView raising the event (incase multiple on form)
    '    Dim selectedTreeview As TreeView = CType(sender, TreeView)

    '    'As the mouse moves over nodes, provide feedback to 
    '    'the user by highlighting the node that is the 
    '    'current drop target
    '    Dim pt As Point = _
    '        CType(sender, TreeView).PointToClient(New Point(e.X, e.Y))
    '    Dim targetNode As TreeNode = selectedTreeview.GetNodeAt(pt)

    '    'See if the targetNode is currently selected, 
    '    'if so no need to validate again
    '    If Not (selectedTreeview.SelectedNode Is targetNode) Then
    '        'Select the    node currently under the cursor
    '        selectedTreeview.SelectedNode = targetNode

    '        'Check that the selected node is not the dropNode and
    '        'also that it is not a child of the dropNode and 
    '        'therefore an invalid target
    '        Dim dropNode As TreeNode = _
    '            CType(e.Data.GetData("System.Windows.Forms.TreeNode"),  _
    '            TreeNode)

    '        Do Until targetNode Is Nothing
    '            If targetNode Is dropNode Then
    '                e.Effect = DragDropEffects.None
    '                Exit Sub
    '            End If
    '            targetNode = targetNode.Parent
    '        Loop
    '        'End If

    '        'Currently selected node is a suitable target
    '        e.Effect = DragDropEffects.Move

    '    End If

    'End Sub

    Public Sub TreeViewList_DragDrop(ByVal sender As System.Object, _
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewList.DragDrop

        'Check that there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode", _
              True) = False Then Exit Sub

        'Get the TreeView raising the event (incase multiple on form)
        Dim selectedTreeview As TreeView = CType(sender, TreeView)

        'Get the TreeNode being dragged
        Dim dropNode As TreeNode = _
              CType(e.Data.GetData("System.Windows.Forms.TreeNode"),  _
              TreeNode)

        'The target node should be selected from the DragOver event
        Dim targetNode As TreeNode = selectedTreeview.SelectedNode

        'Remove the drop node from its current location
        'dropNode.Remove()

        'If there is no targetNode add dropNode to the bottom of
        'the TreeView root nodes, otherwise add it to the end of
        'the dropNode child nodes


        'Modif Olive (Debut)
        If dropNode.Level = 0 Then
            For Each x As TreeNode In dropNode.Nodes
                AddNodes(selectedTreeview, x.Name)
            Next
        Else
            AddNodes(selectedTreeview, dropNode.Name)
        End If

        If dropNode.Level = 0 Then
            dropNode.Remove()
        ElseIf dropNode.Parent.Nodes.Count = 1 Then
            dropNode.Parent.Remove()
        Else
            dropNode.Remove()
        End If
        'Modif Olive (Fin)

        'If targetNode Is Nothing Then
        '    selectedTreeview.Nodes.Add(dropNode)
        'Else
        '    targetNode.Nodes.Add(dropNode)
        'End If

        'Ensure the newley created node is visible to
        'the user and select it
        dropNode.EnsureVisible()
        selectedTreeview.SelectedNode = dropNode

    End Sub


   
    Private Sub ButtonParcourir_Click(sender As System.Object, e As System.EventArgs) Handles ButtonParcourir.Click

        FolderBrowserDialog1.SelectedPath = mPath

        FolderBrowserDialog1.Description = "Please select a data source:" + vbNewLine + mPath

        FolderBrowserDialog1.ShowNewFolderButton = False

        If FolderBrowserDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then

            mPath = FolderBrowserDialog1.SelectedPath

            If Strings.Right(mPath, 1) <> "\" Then mPath &= "\"

            ReadSources()

        End If


    End Sub

    
  
End Class

