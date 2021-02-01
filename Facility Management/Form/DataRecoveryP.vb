Option Strict On
Option Explicit On

Public Class DataRecoveryP

    Private mPath As String
    Private mDestination As String
    Public LaunchDirectClass As Boolean
    Public NameOfWave As String
    Public IsNotLockedTreeView As Boolean = True

    Private ReadOnly databasePresse As New Dictionary(Of String, String)

    Private Sub Init()
        mPath = Select_GetIniString(FormBox.MenuSoft, "Source", FormBox.FacilityManagementIni).Trim
        mDestination = Select_GetIniString(FormBox.MenuSoft, "Destination", FormBox.FacilityManagementIni).Trim
    End Sub

    Delegate Sub MyDelegate(myControl As ComboBox, myArg2 As ArrayList)

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged

        ButtonCreating.Enabled = False

        Dim DroitSelected As String = CStr(ComboBox1.SelectedItem).Trim

        If DroitSelected <> Nothing Then

            TreeViewList.Nodes.Clear()
            TreeViewSelected.Nodes.Clear()

            TreeViewList.Sort()
            TreeViewSelected.Sort()

            databasePresse.Clear()

            System.Windows.Forms.Application.DoEvents()

            Dim SourcesNames() As String = IO.Directory.GetDirectories(mPath, "*.", IO.SearchOption.TopDirectoryOnly)

            For i As Integer = 0 To SourcesNames.Length - 1

                If IO.Directory.Exists(SourcesNames(i) + "\DroitsAcces\" + DroitSelected) Then

                    Dim DirectoryName As String = ParsePath(SourcesNames(i), FILENAME)

                    Dim SourceName As String = ReadSourceName(DirectoryName)


                    If DirectoryName <> "" Then

                        Dim TreeViewSelect As TreeView
                        If IO.Directory.Exists(mDestination & DirectoryName) Then
                            TreeViewSelect = TreeViewSelected
                        Else
                            TreeViewSelect = TreeViewList
                        End If

                        If SourceName <> "" Then
                            AddNodes(TreeViewSelect, SourceName)
                            'Else
                            '    AddNodes(TreeViewSelect, DirectoryName)
                        End If

                    End If

                End If

            Next

        End If

        ButtonCreating.Enabled = True

    End Sub

    Public Sub ReadDroitAcces()

        Dim DroitsAcces() As String = IO.Directory.GetDirectories(mPath, "DroitsAcces", IO.SearchOption.AllDirectories)

        Dim ListDroitAcces As ArrayList = New ArrayList

        'ListDroitAcces.Clear()

        For i As Integer = 0 To DroitsAcces.Length - 1

            Dim ListClients() As String = IO.Directory.GetDirectories(DroitsAcces(i), "*.*", IO.SearchOption.TopDirectoryOnly)

            For Each client As String In ListClients
                If Me.Visible = False Then Exit Sub
                Dim cli As String = ParsePath(client, FILENAME)
                If Not ListDroitAcces.Contains(cli) Then
                    ListDroitAcces.Add(cli)
                End If
            Next
        Next

        Dim myArray(1) As Object

        myArray(0) = ComboBox1 'New ComboBox()
        myArray(1) = ListDroitAcces 'New ArrayList

        ComboBox1.Invoke(New MyDelegate(AddressOf DelegateMethod), myArray)
        'ComboBox1.Invoke(New MyDelegate(AddressOf DelegateMethod), ComboBox1, ListDroitAcces)

    End Sub

    Public Sub DelegateMethod(myControl As ComboBox, ListDroitAcces As ArrayList)

        For Each client As String In ListDroitAcces
            If Me.Visible = False Then Exit Sub
            If Not myControl.Items.Contains(UCase(client)) Then
                myControl.Items.Add(UCase(client))
            End If
        Next

    End Sub 'DelegateMethod

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        Init()

        Me.Opacity = FormBox.nvOpacity

        ComboBox1.Items.Clear()
        ComboBox1.Sorted = True

        ComboBox1.Items.Add("JFC")

        ToolStripStatusLabel1.Text = ""

        Dim DelOpenInt As Action
        DelOpenInt = New Action(AddressOf ReadDroitAcces)
        Dim ResOpenInt = DelOpenInt.BeginInvoke(Nothing, Nothing)


    End Sub

    Private Function ReadSourceName(ByVal SourceName As String) As String

        Dim mSourcesTbl() As String = IO.File.ReadAllLines(mPath & "Sources.tbl", System.Text.Encoding.Default)
        Dim mTerrainsTbl() As String = IO.File.ReadAllLines(mPath & "Terrains.tbl", System.Text.Encoding.Default)

        Dim szTerrain As String = ""

        For Each x As String In mTerrainsTbl
            If x.Contains("/" + SourceName + "/") Then
                szTerrain = x
                Exit For
            End If
        Next

        Dim szSource As String = ""

        If szTerrain <> Nothing Then
            Dim z() As String = Split(szTerrain, ",")
            For Each w As String In mSourcesTbl

                Try

                    Dim v() As String = Split(w, ",")
                    If v.Length = 3 Then
                        If v(0).Trim = z(1).Trim Then
                            szSource = Replace(v(2), Chr(34), "").Trim
                            szTerrain = Replace(z(3), Chr(34), "").Trim
                            Exit For
                        End If
                    End If

                Catch ex As Exception
                    'Ligne incorrect !
                End Try

            Next

        End If

        If szSource <> "" And szTerrain <> "" Then

            databasePresse.Add(szSource + "\" + szTerrain, SourceName)

            Return szSource + "\" + szTerrain

        Else
            Return ""
        End If

    End Function

    Private Sub AddNodes(ByVal node As TreeView, ByVal Libelle As String)
        Dim nPosSlash As Integer = InStr(Libelle, "\", CompareMethod.Text)

        If nPosSlash > 0 Then
            Dim Noeud As String = Mid(Libelle, 1, nPosSlash - 1)
            Libelle = Mid(Libelle, nPosSlash + 1, Len(Libelle) - 1)

            If Not node.Nodes.ContainsKey(Noeud) Then
                node.Nodes.Add(Noeud, Noeud)
            End If

            Dim IndexNoeud As Integer = node.Nodes.IndexOfKey(Noeud)
            If Not node.Nodes(IndexNoeud).Nodes.ContainsKey(Libelle) Then
                node.Nodes(IndexNoeud).Nodes.Add(Libelle, Libelle)
            End If

        End If

    End Sub

    Private Sub TreeViewList_NodeMouseDoubleClick(sender As Object, e As System.Windows.Forms.TreeNodeMouseClickEventArgs) Handles TreeViewList.NodeMouseDoubleClick

        If e.Node.Level = 0 Then
            For Each x As TreeNode In e.Node.Nodes
                AddNodes(TreeViewSelected, e.Node.Name + "\" + x.Name)
            Next
        Else
            AddNodes(TreeViewSelected, e.Node.Parent.Name + "\" + e.Node.Name)
            TreeViewSelected.Nodes(TreeViewSelected.Nodes.IndexOfKey(e.Node.Parent.Text)).Expand()
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
                AddNodes(TreeViewList, e.Node.Name + "\" + x.Name)
            Next
        Else
            AddNodes(TreeViewList, e.Node.Parent.Name + "\" + e.Node.Name)
        End If

        If e.Node.Level = 0 Then
            e.Node.Remove()
        ElseIf e.Node.Parent.Nodes.Count = 1 Then
            e.Node.Parent.Remove()
        Else
            e.Node.Remove()
        End If

    End Sub

    Public Sub TreeViewSelected_ItemDrag(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.ItemDragEventArgs) _
        Handles TreeViewSelected.ItemDrag

        'Set the drag node and initiate the DragDrop 
        DoDragDrop(e.Item, DragDropEffects.Move)

    End Sub

    Public Sub TreeViewSelected_DragEnter(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewSelected.DragEnter

        'See if there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode",
            True) Then
            'TreeNode found allow move effect
            e.Effect = DragDropEffects.Move
        Else
            'No TreeNode found, prevent move
            e.Effect = DragDropEffects.None
        End If

    End Sub

    Public Sub TreeViewSelected_DragDrop(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewSelected.DragDrop

        'Check that there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode",
              True) = False Then Exit Sub

        'Get the TreeView raising the event (incase multiple on form)
        Dim selectedTreeview As TreeView = CType(sender, TreeView)

        'Get the TreeNode being dragged
        Dim dropNode As TreeNode =
              CType(e.Data.GetData("System.Windows.Forms.TreeNode"),
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
                AddNodes(selectedTreeview, dropNode.Name + "\" + x.Name)
            Next
        Else
            AddNodes(selectedTreeview, dropNode.Parent.Name + "\" + dropNode.Name)
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

    Public Sub TreeViewList_ItemDrag(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.ItemDragEventArgs) _
        Handles TreeViewList.ItemDrag

        'Set the drag node and initiate the DragDrop 
        DoDragDrop(e.Item, DragDropEffects.Move)

    End Sub

    Public Sub TreeViewList_DragEnter(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewList.DragEnter

        'See if there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode",
            True) Then
            'TreeNode found allow move effect
            e.Effect = DragDropEffects.Move
        Else
            'No TreeNode found, prevent move
            e.Effect = DragDropEffects.None
        End If

    End Sub

    Public Sub TreeViewList_DragDrop(ByVal sender As System.Object,
        ByVal e As System.Windows.Forms.DragEventArgs) _
        Handles TreeViewList.DragDrop

        'Check that there is a TreeNode being dragged
        If e.Data.GetDataPresent("System.Windows.Forms.TreeNode",
              True) = False Then Exit Sub

        'Get the TreeView raising the event (incase multiple on form)
        Dim selectedTreeview As TreeView = CType(sender, TreeView)

        'Get the TreeNode being dragged
        Dim dropNode As TreeNode =
              CType(e.Data.GetData("System.Windows.Forms.TreeNode"),
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
                AddNodes(selectedTreeview, dropNode.Name + "\" + x.Name)
            Next
        Else
            AddNodes(selectedTreeview, dropNode.Parent.Name + "\" + dropNode.Name)
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

    Private Sub CopyFiles(ByVal mSrc As String, ByVal mDest As String, Optional ByVal SearchFile As String = "*.*", Optional ByVal Directory As System.IO.SearchOption = IO.SearchOption.TopDirectoryOnly)

        Dim ListFiles() As String = IO.Directory.GetFiles(mSrc, SearchFile, Directory)

        Dim mSrcFile As String, mDestFile As String, mDestDirectory As String

        If Strings.Right(mSrc, 1) <> "\" Then mSrc &= "\"

        If Strings.Right(mDest, 1) <> "\" Then mDest &= "\"

        For y As Integer = 0 To ListFiles.Length - 1
            mSrcFile = ListFiles(y)
            mDestFile = mDest & Replace(mSrcFile, mSrc, "")
            mDestDirectory = ParsePath(mDestFile, PATH)
            If Not IO.Directory.Exists(mDestDirectory) Then
                IO.Directory.CreateDirectory(mDestDirectory)
            End If

            Dim nPosLastSlach As Integer = InStrRev(mDestFile, "\")
            ToolStripStatusLabel1.Text = Mid(mDestFile, nPosLastSlach + 1, Len(mDestFile) - nPosLastSlach)

            ToolStripProgressBar1.Value = ToolStripProgressBar1.Value + 1

            System.Windows.Forms.Application.DoEvents()
            IO.File.Copy(mSrcFile, mDestFile, True)
        Next


    End Sub

    Private Function CountFiles() As Integer

        Dim mRepSrc As String = mPath & "\"

        CountFiles = 0

        For Each u As TreeNode In TreeViewSelected.Nodes
            For Each x As TreeNode In u.Nodes

                Dim mSourceName As String = databasePresse(u.Name + "\" + x.Name)

                Dim ListFilesVague() As String = IO.Directory.GetFiles(mRepSrc & mSourceName, "*.*", IO.SearchOption.TopDirectoryOnly)

                CountFiles += ListFilesVague.Length

                Dim ListFilesAcces() As String = IO.Directory.GetFiles(mRepSrc & mSourceName + "\DroitsAcces\" + CStr(ComboBox1.SelectedItem).Trim, "*.*", IO.SearchOption.TopDirectoryOnly)

                CountFiles += ListFilesAcces.Length

            Next
        Next

    End Function

    Private Sub ButtonCreating_Click(sender As System.Object, e As System.EventArgs) Handles ButtonCreating.Click
        ButtonCreating.Enabled = False

        ToolStripProgressBar1.Maximum = CountFiles()
        ToolStripProgressBar1.Value = 0

        IO.File.Delete(mDestination & "Sources.new")
        IO.File.Delete(mDestination & "Terrains.new")

        NameOfWave = "Terrains:" + vbNewLine

        For Each u As TreeNode In TreeViewSelected.Nodes
            For Each x As TreeNode In u.Nodes

                Dim mSourceName As String = databasePresse(u.Name + "\" + x.Name)

                CopyFiles(mPath & mSourceName, mDestination & mSourceName, "*.*", IO.SearchOption.TopDirectoryOnly)

                'DroitAcces
                CopyFiles(mPath + mSourceName + "\DroitsAcces\" + CStr(ComboBox1.SelectedItem).Trim, mDestination + mSourceName, "*.*", IO.SearchOption.TopDirectoryOnly)

                AddLineToFile(mDestination & "Sources.new", FindSourceTbl(u.Name))
                AddLineToFile(mDestination & "Terrains.new", FindTerrainTbl(mSourceName))

                NameOfWave = NameOfWave + ("- " + u.Name + " " + x.Name) + vbNewLine

            Next
        Next

        For Each u As TreeNode In TreeViewList.Nodes
            For Each x As TreeNode In u.Nodes

                Dim mSourceName As String = databasePresse(u.Name + "\" + x.Name)

                If IO.Directory.Exists(mDestination & mSourceName) Then
                    IO.Directory.Delete(mDestination & mSourceName, True)
                End If

            Next
        Next

        IO.File.Copy(mPath & "Terrains.tbl", mDestination & "Terrains.maj", True)
        IO.File.Copy(mPath & "Sources.tbl", mDestination & "Sources.maj", True)

        MsgBox("Upgrade Completed.", MsgBoxStyle.Information)

        ToolStripStatusLabel1.Text = ""
        ToolStripProgressBar1.Value = 0
        ButtonCreating.Enabled = True

    End Sub

    Private Sub ButtonExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonExit.Click
        Me.Close()
    End Sub

    Private Function FindTerrainTbl(ByVal TerrainName As String) As String

        Dim mTerrainsTbl() As String = IO.File.ReadAllLines(mPath & "Terrains.tbl", System.Text.Encoding.Default)

        For Each x As String In mTerrainsTbl
            If x.Contains("/" + TerrainName + "/") Then
                Return x
                Exit For
            End If
        Next

        Return ""

    End Function

    Private Function FindSourceTbl(ByVal SourceName As String) As String

        Dim mSourcesTbl() As String = IO.File.ReadAllLines(mPath & "Sources.tbl", System.Text.Encoding.Default)

        For Each x As String In mSourcesTbl
            If x.Contains(Chr(34) + SourceName + Chr(34)) Then
                Return x
                Exit For
            End If
        Next

        Return ""


    End Function

    Public Sub AddLineToFile(ByVal szFile As String, svAddLine As String)

        If svAddLine <> "" Then
            If IO.File.Exists(szFile) Then

                Dim mFile() As String = IO.File.ReadAllLines(szFile, System.Text.Encoding.Default)

                If Not mFile.Contains(svAddLine) Then

                    Dim s As Integer
                    For s = mFile.Length - 1 To 0 Step -1
                        If mFile(s).Trim <> Nothing Then Exit For
                    Next

                    ReDim Preserve mFile(s + 1)
                    mFile(s + 1) = svAddLine


                    'If mFile(mFile.Length - 1) = Nothing Then
                    '    mFile(mFile.Length - 1) = svAddLine
                    'Else
                    '    ReDim Preserve mFile(mFile.Length)
                    '    mFile(mFile.Length - 1) = svAddLine
                    'End If

                    'While mFile(mFile.Length - 1).Trim = Nothing

                    '    ReDim mFile(mFile.Length - 2)

                    'End While

                    IO.File.WriteAllLines(szFile, mFile, System.Text.Encoding.Default)

                End If
            Else
                IO.File.WriteAllText(szFile, svAddLine, System.Text.Encoding.Default)
            End If

        End If

    End Sub

End Class