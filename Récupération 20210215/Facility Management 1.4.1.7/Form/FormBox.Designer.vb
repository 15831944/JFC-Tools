<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FormBox
    Inherits System.Windows.Forms.Form

    'Form remplace la méthode Dispose pour nettoyer la liste des composants.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Requise par le Concepteur Windows Form
    Private components As System.ComponentModel.IContainer

    'REMARQUE : la procédure suivante est requise par le Concepteur Windows Form
    'Elle peut être modifiée à l'aide du Concepteur Windows Form.  
    'Ne la modifiez pas à l'aide de l'éditeur de code.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(FormBox))
        Me.CMS_ListPatch = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.CMS_ListSelectClient = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.CMS_ListSelectPatch = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.CMS_ListAccesClient = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.MenuStripFM = New System.Windows.Forms.MenuStrip()
        Me.StatusStripFM = New System.Windows.Forms.StatusStrip()
        Me.ToolStripProgressBar1 = New System.Windows.Forms.ToolStripProgressBar()
        Me.ToolStripStatusLabelFM = New System.Windows.Forms.ToolStripStatusLabel()
        Me.ToolStripStatusLabelDisk = New System.Windows.Forms.ToolStripStatusLabel()
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.Label_version = New System.Windows.Forms.Label()
        Me.ListSelectClient = New System.Windows.Forms.ListBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ListAccesClient = New System.Windows.Forms.ListBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.ListSelectPatch = New System.Windows.Forms.ListBox()
        Me.SplitContainer1 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer2 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer3 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer4 = New System.Windows.Forms.SplitContainer()
        Me.ButtonFiltreUpdates = New System.Windows.Forms.Button()
        Me.ListPatch = New System.Windows.Forms.ListBox()
        Me.FiltrePatch = New System.Windows.Forms.TextBox()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.SplitContainer5 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer7 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer8 = New System.Windows.Forms.SplitContainer()
        Me.F2 = New System.Windows.Forms.Button()
        Me.F3 = New System.Windows.Forms.Button()
        Me.SplitContainer9 = New System.Windows.Forms.SplitContainer()
        Me.F4 = New System.Windows.Forms.Button()
        Me.F5 = New System.Windows.Forms.Button()
        Me.SplitContainer6 = New System.Windows.Forms.SplitContainer()
        Me.SplitContainer10 = New System.Windows.Forms.SplitContainer()
        Me.F6 = New System.Windows.Forms.Button()
        Me.F9 = New System.Windows.Forms.Button()
        Me.F10 = New System.Windows.Forms.Button()
        Me.FolderBrowserDialogZip = New System.Windows.Forms.FolderBrowserDialog()
        Me.OpenFilePatchList = New System.Windows.Forms.OpenFileDialog()
        Me.StatusStripFM.SuspendLayout()
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer1.Panel1.SuspendLayout()
        Me.SplitContainer1.Panel2.SuspendLayout()
        Me.SplitContainer1.SuspendLayout()
        CType(Me.SplitContainer2, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer2.Panel1.SuspendLayout()
        Me.SplitContainer2.Panel2.SuspendLayout()
        Me.SplitContainer2.SuspendLayout()
        CType(Me.SplitContainer3, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer3.Panel1.SuspendLayout()
        Me.SplitContainer3.Panel2.SuspendLayout()
        Me.SplitContainer3.SuspendLayout()
        CType(Me.SplitContainer4, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer4.Panel1.SuspendLayout()
        Me.SplitContainer4.Panel2.SuspendLayout()
        Me.SplitContainer4.SuspendLayout()
        CType(Me.SplitContainer5, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer5.Panel1.SuspendLayout()
        Me.SplitContainer5.Panel2.SuspendLayout()
        Me.SplitContainer5.SuspendLayout()
        CType(Me.SplitContainer7, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer7.Panel1.SuspendLayout()
        Me.SplitContainer7.Panel2.SuspendLayout()
        Me.SplitContainer7.SuspendLayout()
        CType(Me.SplitContainer8, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer8.Panel1.SuspendLayout()
        Me.SplitContainer8.Panel2.SuspendLayout()
        Me.SplitContainer8.SuspendLayout()
        CType(Me.SplitContainer9, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer9.Panel1.SuspendLayout()
        Me.SplitContainer9.Panel2.SuspendLayout()
        Me.SplitContainer9.SuspendLayout()
        CType(Me.SplitContainer6, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer6.Panel1.SuspendLayout()
        Me.SplitContainer6.Panel2.SuspendLayout()
        Me.SplitContainer6.SuspendLayout()
        CType(Me.SplitContainer10, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer10.Panel1.SuspendLayout()
        Me.SplitContainer10.Panel2.SuspendLayout()
        Me.SplitContainer10.SuspendLayout()
        Me.SuspendLayout()
        '
        'CMS_ListPatch
        '
        Me.CMS_ListPatch.Name = "CMS_ListPatch"
        Me.CMS_ListPatch.Size = New System.Drawing.Size(61, 4)
        '
        'CMS_ListSelectClient
        '
        Me.CMS_ListSelectClient.Name = "CMS_ListSelectClient"
        Me.CMS_ListSelectClient.Size = New System.Drawing.Size(61, 4)
        '
        'CMS_ListSelectPatch
        '
        Me.CMS_ListSelectPatch.Name = "CMS_ListSelectPatch"
        Me.CMS_ListSelectPatch.Size = New System.Drawing.Size(61, 4)
        '
        'CMS_ListAccesClient
        '
        Me.CMS_ListAccesClient.Name = "CMS_ListAccesClient"
        Me.CMS_ListAccesClient.Size = New System.Drawing.Size(61, 4)
        '
        'MenuStripFM
        '
        Me.MenuStripFM.AutoSize = False
        Me.MenuStripFM.GripMargin = New System.Windows.Forms.Padding(0)
        Me.MenuStripFM.Location = New System.Drawing.Point(0, 0)
        Me.MenuStripFM.Name = "MenuStripFM"
        Me.MenuStripFM.Padding = New System.Windows.Forms.Padding(0, 2, 0, 2)
        Me.MenuStripFM.Size = New System.Drawing.Size(776, 40)
        Me.MenuStripFM.TabIndex = 21
        Me.MenuStripFM.Text = "MenuStrip1"
        '
        'StatusStripFM
        '
        Me.StatusStripFM.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripProgressBar1, Me.ToolStripStatusLabelFM, Me.ToolStripStatusLabelDisk})
        Me.StatusStripFM.Location = New System.Drawing.Point(0, 439)
        Me.StatusStripFM.Name = "StatusStripFM"
        Me.StatusStripFM.Size = New System.Drawing.Size(776, 22)
        Me.StatusStripFM.TabIndex = 22
        Me.StatusStripFM.Text = "StatusStrip1"
        '
        'ToolStripProgressBar1
        '
        Me.ToolStripProgressBar1.Name = "ToolStripProgressBar1"
        Me.ToolStripProgressBar1.Size = New System.Drawing.Size(150, 16)
        Me.ToolStripProgressBar1.Step = 1
        Me.ToolStripProgressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous
        '
        'ToolStripStatusLabelFM
        '
        Me.ToolStripStatusLabelFM.Name = "ToolStripStatusLabelFM"
        Me.ToolStripStatusLabelFM.Size = New System.Drawing.Size(130, 17)
        Me.ToolStripStatusLabelFM.Text = "ToolStripStatusLabelFM"
        '
        'ToolStripStatusLabelDisk
        '
        Me.ToolStripStatusLabelDisk.Name = "ToolStripStatusLabelDisk"
        Me.ToolStripStatusLabelDisk.Size = New System.Drawing.Size(135, 17)
        Me.ToolStripStatusLabelDisk.Text = "ToolStripStatusLabelDisk"
        '
        'Label_version
        '
        Me.Label_version.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Label_version.AutoSize = True
        Me.Label_version.Location = New System.Drawing.Point(726, 444)
        Me.Label_version.Name = "Label_version"
        Me.Label_version.Size = New System.Drawing.Size(46, 13)
        Me.Label_version.TabIndex = 24
        Me.Label_version.Text = "v1.0.0.0"
        '
        'ListSelectClient
        '
        Me.ListSelectClient.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ListSelectClient.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.ListSelectClient.ContextMenuStrip = Me.CMS_ListSelectClient
        Me.ListSelectClient.Cursor = System.Windows.Forms.Cursors.Default
        Me.ListSelectClient.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListSelectClient.FormattingEnabled = True
        Me.ListSelectClient.IntegralHeight = False
        Me.ListSelectClient.ItemHeight = 16
        Me.ListSelectClient.Location = New System.Drawing.Point(3, 21)
        Me.ListSelectClient.Name = "ListSelectClient"
        Me.ListSelectClient.Size = New System.Drawing.Size(181, 331)
        Me.ListSelectClient.TabIndex = 25
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!)
        Me.Label1.Location = New System.Drawing.Point(3, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(105, 18)
        Me.Label1.TabIndex = 26
        Me.Label1.Text = "Customer List:"
        '
        'ListAccesClient
        '
        Me.ListAccesClient.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ListAccesClient.BackColor = System.Drawing.SystemColors.Window
        Me.ListAccesClient.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.ListAccesClient.ContextMenuStrip = Me.CMS_ListAccesClient
        Me.ListAccesClient.Cursor = System.Windows.Forms.Cursors.Default
        Me.ListAccesClient.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListAccesClient.FormattingEnabled = True
        Me.ListAccesClient.IntegralHeight = False
        Me.ListAccesClient.ItemHeight = 16
        Me.ListAccesClient.Location = New System.Drawing.Point(3, 21)
        Me.ListAccesClient.Name = "ListAccesClient"
        Me.ListAccesClient.Size = New System.Drawing.Size(179, 331)
        Me.ListAccesClient.TabIndex = 25
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!)
        Me.Label2.Location = New System.Drawing.Point(3, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(144, 18)
        Me.Label2.TabIndex = 27
        Me.Label2.Text = "Selected customers:"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!)
        Me.Label3.Location = New System.Drawing.Point(3, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(128, 18)
        Me.Label3.TabIndex = 28
        Me.Label3.Text = "Selected Updates:"
        '
        'ListSelectPatch
        '
        Me.ListSelectPatch.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ListSelectPatch.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.ListSelectPatch.ContextMenuStrip = Me.CMS_ListSelectPatch
        Me.ListSelectPatch.Cursor = System.Windows.Forms.Cursors.Default
        Me.ListSelectPatch.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListSelectPatch.FormattingEnabled = True
        Me.ListSelectPatch.IntegralHeight = False
        Me.ListSelectPatch.ItemHeight = 16
        Me.ListSelectPatch.Location = New System.Drawing.Point(3, 21)
        Me.ListSelectPatch.Name = "ListSelectPatch"
        Me.ListSelectPatch.Size = New System.Drawing.Size(178, 331)
        Me.ListSelectPatch.TabIndex = 29
        '
        'SplitContainer1
        '
        Me.SplitContainer1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.SplitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2
        Me.SplitContainer1.IsSplitterFixed = True
        Me.SplitContainer1.Location = New System.Drawing.Point(12, 43)
        Me.SplitContainer1.Name = "SplitContainer1"
        Me.SplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal
        '
        'SplitContainer1.Panel1
        '
        Me.SplitContainer1.Panel1.Controls.Add(Me.SplitContainer2)
        '
        'SplitContainer1.Panel2
        '
        Me.SplitContainer1.Panel2.Controls.Add(Me.SplitContainer5)
        Me.SplitContainer1.Size = New System.Drawing.Size(752, 393)
        Me.SplitContainer1.SplitterDistance = 355
        Me.SplitContainer1.TabIndex = 35
        '
        'SplitContainer2
        '
        Me.SplitContainer2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer2.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer2.Name = "SplitContainer2"
        '
        'SplitContainer2.Panel1
        '
        Me.SplitContainer2.Panel1.Controls.Add(Me.SplitContainer3)
        '
        'SplitContainer2.Panel2
        '
        Me.SplitContainer2.Panel2.Controls.Add(Me.SplitContainer4)
        Me.SplitContainer2.Size = New System.Drawing.Size(752, 355)
        Me.SplitContainer2.SplitterDistance = 376
        Me.SplitContainer2.TabIndex = 0
        '
        'SplitContainer3
        '
        Me.SplitContainer3.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer3.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer3.Name = "SplitContainer3"
        '
        'SplitContainer3.Panel1
        '
        Me.SplitContainer3.Panel1.Controls.Add(Me.ListAccesClient)
        Me.SplitContainer3.Panel1.Controls.Add(Me.Label1)
        '
        'SplitContainer3.Panel2
        '
        Me.SplitContainer3.Panel2.Controls.Add(Me.ListSelectClient)
        Me.SplitContainer3.Panel2.Controls.Add(Me.Label2)
        Me.SplitContainer3.Size = New System.Drawing.Size(376, 355)
        Me.SplitContainer3.SplitterDistance = 185
        Me.SplitContainer3.TabIndex = 0
        '
        'SplitContainer4
        '
        Me.SplitContainer4.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer4.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer4.Name = "SplitContainer4"
        '
        'SplitContainer4.Panel1
        '
        Me.SplitContainer4.Panel1.Controls.Add(Me.Label3)
        Me.SplitContainer4.Panel1.Controls.Add(Me.ListSelectPatch)
        '
        'SplitContainer4.Panel2
        '
        Me.SplitContainer4.Panel2.Controls.Add(Me.ButtonFiltreUpdates)
        Me.SplitContainer4.Panel2.Controls.Add(Me.ListPatch)
        Me.SplitContainer4.Panel2.Controls.Add(Me.FiltrePatch)
        Me.SplitContainer4.Panel2.Controls.Add(Me.Label5)
        Me.SplitContainer4.Panel2.Controls.Add(Me.Label4)
        Me.SplitContainer4.Size = New System.Drawing.Size(372, 355)
        Me.SplitContainer4.SplitterDistance = 184
        Me.SplitContainer4.TabIndex = 0
        '
        'ButtonFiltreUpdates
        '
        Me.ButtonFiltreUpdates.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ButtonFiltreUpdates.FlatAppearance.BorderSize = 0
        Me.ButtonFiltreUpdates.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.ButtonFiltreUpdates.Location = New System.Drawing.Point(160, 325)
        Me.ButtonFiltreUpdates.Name = "ButtonFiltreUpdates"
        Me.ButtonFiltreUpdates.Size = New System.Drawing.Size(21, 27)
        Me.ButtonFiltreUpdates.TabIndex = 34
        Me.ButtonFiltreUpdates.Text = "X"
        Me.ButtonFiltreUpdates.UseVisualStyleBackColor = True
        '
        'ListPatch
        '
        Me.ListPatch.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ListPatch.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.ListPatch.ContextMenuStrip = Me.CMS_ListPatch
        Me.ListPatch.Cursor = System.Windows.Forms.Cursors.Default
        Me.ListPatch.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ListPatch.FormattingEnabled = True
        Me.ListPatch.IntegralHeight = False
        Me.ListPatch.ItemHeight = 16
        Me.ListPatch.Location = New System.Drawing.Point(3, 21)
        Me.ListPatch.Name = "ListPatch"
        Me.ListPatch.Size = New System.Drawing.Size(178, 292)
        Me.ListPatch.TabIndex = 30
        '
        'FiltrePatch
        '
        Me.FiltrePatch.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.FiltrePatch.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.FiltrePatch.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FiltrePatch.Location = New System.Drawing.Point(3, 337)
        Me.FiltrePatch.Name = "FiltrePatch"
        Me.FiltrePatch.Size = New System.Drawing.Size(151, 15)
        Me.FiltrePatch.TabIndex = 33
        '
        'Label5
        '
        Me.Label5.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!)
        Me.Label5.Location = New System.Drawing.Point(3, 316)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(99, 18)
        Me.Label5.TabIndex = 32
        Me.Label5.Text = "Filter Updates"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!)
        Me.Label4.Location = New System.Drawing.Point(3, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(94, 18)
        Me.Label4.TabIndex = 31
        Me.Label4.Text = "Updates List:"
        '
        'SplitContainer5
        '
        Me.SplitContainer5.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer5.IsSplitterFixed = True
        Me.SplitContainer5.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer5.Name = "SplitContainer5"
        '
        'SplitContainer5.Panel1
        '
        Me.SplitContainer5.Panel1.Controls.Add(Me.SplitContainer7)
        '
        'SplitContainer5.Panel2
        '
        Me.SplitContainer5.Panel2.Controls.Add(Me.SplitContainer6)
        Me.SplitContainer5.Size = New System.Drawing.Size(752, 34)
        Me.SplitContainer5.SplitterDistance = 429
        Me.SplitContainer5.TabIndex = 0
        '
        'SplitContainer7
        '
        Me.SplitContainer7.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer7.IsSplitterFixed = True
        Me.SplitContainer7.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer7.Name = "SplitContainer7"
        '
        'SplitContainer7.Panel1
        '
        Me.SplitContainer7.Panel1.Controls.Add(Me.SplitContainer8)
        '
        'SplitContainer7.Panel2
        '
        Me.SplitContainer7.Panel2.Controls.Add(Me.SplitContainer9)
        Me.SplitContainer7.Size = New System.Drawing.Size(429, 34)
        Me.SplitContainer7.SplitterDistance = 214
        Me.SplitContainer7.TabIndex = 0
        '
        'SplitContainer8
        '
        Me.SplitContainer8.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer8.IsSplitterFixed = True
        Me.SplitContainer8.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer8.Name = "SplitContainer8"
        '
        'SplitContainer8.Panel1
        '
        Me.SplitContainer8.Panel1.Controls.Add(Me.F2)
        '
        'SplitContainer8.Panel2
        '
        Me.SplitContainer8.Panel2.Controls.Add(Me.F3)
        Me.SplitContainer8.Size = New System.Drawing.Size(214, 34)
        Me.SplitContainer8.SplitterDistance = 107
        Me.SplitContainer8.TabIndex = 0
        '
        'F2
        '
        Me.F2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F2.AutoSize = True
        Me.F2.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F2.Location = New System.Drawing.Point(3, 3)
        Me.F2.Name = "F2"
        Me.F2.Size = New System.Drawing.Size(101, 28)
        Me.F2.TabIndex = 35
        Me.F2.Text = "F2"
        Me.F2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F2.UseVisualStyleBackColor = True
        '
        'F3
        '
        Me.F3.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F3.AutoSize = True
        Me.F3.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F3.Location = New System.Drawing.Point(2, 3)
        Me.F3.Name = "F3"
        Me.F3.Size = New System.Drawing.Size(98, 28)
        Me.F3.TabIndex = 36
        Me.F3.Text = "F3"
        Me.F3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F3.UseVisualStyleBackColor = True
        '
        'SplitContainer9
        '
        Me.SplitContainer9.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer9.IsSplitterFixed = True
        Me.SplitContainer9.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer9.Name = "SplitContainer9"
        '
        'SplitContainer9.Panel1
        '
        Me.SplitContainer9.Panel1.Controls.Add(Me.F4)
        '
        'SplitContainer9.Panel2
        '
        Me.SplitContainer9.Panel2.Controls.Add(Me.F5)
        Me.SplitContainer9.Size = New System.Drawing.Size(211, 34)
        Me.SplitContainer9.SplitterDistance = 105
        Me.SplitContainer9.TabIndex = 0
        '
        'F4
        '
        Me.F4.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F4.AutoSize = True
        Me.F4.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F4.Location = New System.Drawing.Point(3, 3)
        Me.F4.Name = "F4"
        Me.F4.Size = New System.Drawing.Size(99, 28)
        Me.F4.TabIndex = 37
        Me.F4.Text = "F4"
        Me.F4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F4.UseVisualStyleBackColor = True
        '
        'F5
        '
        Me.F5.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F5.AutoSize = True
        Me.F5.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F5.Location = New System.Drawing.Point(3, 3)
        Me.F5.Name = "F5"
        Me.F5.Size = New System.Drawing.Size(96, 28)
        Me.F5.TabIndex = 38
        Me.F5.Text = "F5"
        Me.F5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F5.UseVisualStyleBackColor = True
        '
        'SplitContainer6
        '
        Me.SplitContainer6.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer6.IsSplitterFixed = True
        Me.SplitContainer6.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer6.Name = "SplitContainer6"
        '
        'SplitContainer6.Panel1
        '
        Me.SplitContainer6.Panel1.Controls.Add(Me.SplitContainer10)
        '
        'SplitContainer6.Panel2
        '
        Me.SplitContainer6.Panel2.Controls.Add(Me.F10)
        Me.SplitContainer6.Size = New System.Drawing.Size(319, 34)
        Me.SplitContainer6.SplitterDistance = 212
        Me.SplitContainer6.TabIndex = 0
        '
        'SplitContainer10
        '
        Me.SplitContainer10.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer10.IsSplitterFixed = True
        Me.SplitContainer10.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer10.Name = "SplitContainer10"
        '
        'SplitContainer10.Panel1
        '
        Me.SplitContainer10.Panel1.Controls.Add(Me.F6)
        '
        'SplitContainer10.Panel2
        '
        Me.SplitContainer10.Panel2.Controls.Add(Me.F9)
        Me.SplitContainer10.Size = New System.Drawing.Size(212, 34)
        Me.SplitContainer10.SplitterDistance = 106
        Me.SplitContainer10.TabIndex = 0
        '
        'F6
        '
        Me.F6.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F6.AutoSize = True
        Me.F6.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F6.Location = New System.Drawing.Point(3, 3)
        Me.F6.Name = "F6"
        Me.F6.Size = New System.Drawing.Size(100, 28)
        Me.F6.TabIndex = 39
        Me.F6.Text = "F6"
        Me.F6.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F6.UseVisualStyleBackColor = True
        '
        'F9
        '
        Me.F9.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F9.AutoSize = True
        Me.F9.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F9.Location = New System.Drawing.Point(3, 3)
        Me.F9.Name = "F9"
        Me.F9.Size = New System.Drawing.Size(96, 28)
        Me.F9.TabIndex = 40
        Me.F9.Text = "F9"
        Me.F9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F9.UseVisualStyleBackColor = True
        '
        'F10
        '
        Me.F10.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.F10.AutoSize = True
        Me.F10.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.F10.Location = New System.Drawing.Point(3, 3)
        Me.F10.Name = "F10"
        Me.F10.Size = New System.Drawing.Size(97, 28)
        Me.F10.TabIndex = 41
        Me.F10.Text = "F10"
        Me.F10.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.F10.UseVisualStyleBackColor = True
        '
        'OpenFilePatchList
        '
        Me.OpenFilePatchList.FileName = "OpenFilePatchList"
        '
        'FormBox
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(776, 461)
        Me.Controls.Add(Me.SplitContainer1)
        Me.Controls.Add(Me.Label_version)
        Me.Controls.Add(Me.StatusStripFM)
        Me.Controls.Add(Me.MenuStripFM)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.KeyPreview = True
        Me.MainMenuStrip = Me.MenuStripFM
        Me.MinimumSize = New System.Drawing.Size(792, 500)
        Me.Name = "FormBox"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Facility Management"
        Me.StatusStripFM.ResumeLayout(False)
        Me.StatusStripFM.PerformLayout()
        Me.SplitContainer1.Panel1.ResumeLayout(False)
        Me.SplitContainer1.Panel2.ResumeLayout(False)
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer1.ResumeLayout(False)
        Me.SplitContainer2.Panel1.ResumeLayout(False)
        Me.SplitContainer2.Panel2.ResumeLayout(False)
        CType(Me.SplitContainer2, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer2.ResumeLayout(False)
        Me.SplitContainer3.Panel1.ResumeLayout(False)
        Me.SplitContainer3.Panel1.PerformLayout()
        Me.SplitContainer3.Panel2.ResumeLayout(False)
        Me.SplitContainer3.Panel2.PerformLayout()
        CType(Me.SplitContainer3, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer3.ResumeLayout(False)
        Me.SplitContainer4.Panel1.ResumeLayout(False)
        Me.SplitContainer4.Panel1.PerformLayout()
        Me.SplitContainer4.Panel2.ResumeLayout(False)
        Me.SplitContainer4.Panel2.PerformLayout()
        CType(Me.SplitContainer4, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer4.ResumeLayout(False)
        Me.SplitContainer5.Panel1.ResumeLayout(False)
        Me.SplitContainer5.Panel2.ResumeLayout(False)
        CType(Me.SplitContainer5, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer5.ResumeLayout(False)
        Me.SplitContainer7.Panel1.ResumeLayout(False)
        Me.SplitContainer7.Panel2.ResumeLayout(False)
        CType(Me.SplitContainer7, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer7.ResumeLayout(False)
        Me.SplitContainer8.Panel1.ResumeLayout(False)
        Me.SplitContainer8.Panel1.PerformLayout()
        Me.SplitContainer8.Panel2.ResumeLayout(False)
        Me.SplitContainer8.Panel2.PerformLayout()
        CType(Me.SplitContainer8, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer8.ResumeLayout(False)
        Me.SplitContainer9.Panel1.ResumeLayout(False)
        Me.SplitContainer9.Panel1.PerformLayout()
        Me.SplitContainer9.Panel2.ResumeLayout(False)
        Me.SplitContainer9.Panel2.PerformLayout()
        CType(Me.SplitContainer9, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer9.ResumeLayout(False)
        Me.SplitContainer6.Panel1.ResumeLayout(False)
        Me.SplitContainer6.Panel2.ResumeLayout(False)
        Me.SplitContainer6.Panel2.PerformLayout()
        CType(Me.SplitContainer6, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer6.ResumeLayout(False)
        Me.SplitContainer10.Panel1.ResumeLayout(False)
        Me.SplitContainer10.Panel1.PerformLayout()
        Me.SplitContainer10.Panel2.ResumeLayout(False)
        Me.SplitContainer10.Panel2.PerformLayout()
        CType(Me.SplitContainer10, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer10.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents MenuStripFM As System.Windows.Forms.MenuStrip
    Friend WithEvents CMS_ListAccesClient As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents StatusStripFM As System.Windows.Forms.StatusStrip
    Friend WithEvents ToolStripProgressBar1 As System.Windows.Forms.ToolStripProgressBar
    Friend WithEvents CMS_ListPatch As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents CMS_ListSelectPatch As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents CMS_ListSelectClient As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents ToolStripStatusLabelFM As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents ToolTip1 As System.Windows.Forms.ToolTip
    Friend WithEvents ToolStripStatusLabelDisk As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents Label_version As System.Windows.Forms.Label
    Friend WithEvents ListSelectClient As System.Windows.Forms.ListBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents ListAccesClient As System.Windows.Forms.ListBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents ListSelectPatch As System.Windows.Forms.ListBox
    Friend WithEvents SplitContainer1 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer2 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer3 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer4 As System.Windows.Forms.SplitContainer
    Friend WithEvents ButtonFiltreUpdates As System.Windows.Forms.Button
    Friend WithEvents ListPatch As System.Windows.Forms.ListBox
    Friend WithEvents FiltrePatch As System.Windows.Forms.TextBox
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents SplitContainer5 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer7 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer8 As System.Windows.Forms.SplitContainer
    Friend WithEvents F2 As System.Windows.Forms.Button
    Friend WithEvents F3 As System.Windows.Forms.Button
    Friend WithEvents SplitContainer9 As System.Windows.Forms.SplitContainer
    Friend WithEvents F4 As System.Windows.Forms.Button
    Friend WithEvents F5 As System.Windows.Forms.Button
    Friend WithEvents SplitContainer6 As System.Windows.Forms.SplitContainer
    Friend WithEvents SplitContainer10 As System.Windows.Forms.SplitContainer
    Friend WithEvents F6 As System.Windows.Forms.Button
    Friend WithEvents F9 As System.Windows.Forms.Button
    Friend WithEvents F10 As System.Windows.Forms.Button
    Friend WithEvents FolderBrowserDialogZip As FolderBrowserDialog
    Friend WithEvents OpenFilePatchList As OpenFileDialog
End Class
