<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class CheckUpdates
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
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(CheckUpdates))
        Me.ButtonExit = New System.Windows.Forms.Button
        Me.Logiciel = New System.Windows.Forms.ComboBox
        Me.ProgressBar1 = New System.Windows.Forms.ProgressBar
        Me.ClicDroit = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.SuspendLayout()
        '
        'ButtonExit
        '
        Me.ButtonExit.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ButtonExit.AutoSize = True
        Me.ButtonExit.FlatAppearance.BorderSize = 0
        Me.ButtonExit.Font = New System.Drawing.Font("Comic Sans MS", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonExit.Location = New System.Drawing.Point(282, 330)
        Me.ButtonExit.Name = "ButtonExit"
        Me.ButtonExit.Size = New System.Drawing.Size(100, 30)
        Me.ButtonExit.TabIndex = 25
        Me.ButtonExit.Text = "&Exit"
        Me.ButtonExit.UseVisualStyleBackColor = True
        '
        'Logiciel
        '
        Me.Logiciel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.Logiciel.FormattingEnabled = True
        Me.Logiciel.ImeMode = System.Windows.Forms.ImeMode.Off
        Me.Logiciel.Location = New System.Drawing.Point(12, 12)
        Me.Logiciel.Name = "Logiciel"
        Me.Logiciel.Size = New System.Drawing.Size(370, 21)
        Me.Logiciel.Sorted = True
        Me.Logiciel.TabIndex = 26
        '
        'ProgressBar1
        '
        Me.ProgressBar1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.ProgressBar1.Location = New System.Drawing.Point(12, 330)
        Me.ProgressBar1.Name = "ProgressBar1"
        Me.ProgressBar1.Size = New System.Drawing.Size(264, 30)
        Me.ProgressBar1.Step = 1
        Me.ProgressBar1.TabIndex = 27
        '
        'ClicDroit
        '
        Me.ClicDroit.Name = "ClicDroit"
        Me.ClicDroit.Size = New System.Drawing.Size(61, 4)
        '
        'CheckUpdates
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(394, 372)
        Me.Controls.Add(Me.ProgressBar1)
        Me.Controls.Add(Me.Logiciel)
        Me.Controls.Add(Me.ButtonExit)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "CheckUpdates"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Check Updates"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ButtonExit As System.Windows.Forms.Button
    Friend WithEvents Logiciel As System.Windows.Forms.ComboBox
    Friend WithEvents ProgressBar1 As System.Windows.Forms.ProgressBar
    Friend WithEvents ClicDroit As System.Windows.Forms.ContextMenuStrip

End Class
