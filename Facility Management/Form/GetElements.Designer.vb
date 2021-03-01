<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class GetElements
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
        Me.ComboBoxVersion = New System.Windows.Forms.ComboBox()
        Me.ChooseLabel = New System.Windows.Forms.Label()
        Me.ButtonCancel = New System.Windows.Forms.Button()
        Me.ButtonGet = New System.Windows.Forms.Button()
        Me.ButtonIgnore = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'ComboBoxVersion
        '
        Me.ComboBoxVersion.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxVersion.FormattingEnabled = True
        Me.ComboBoxVersion.Location = New System.Drawing.Point(12, 30)
        Me.ComboBoxVersion.Name = "ComboBoxVersion"
        Me.ComboBoxVersion.Size = New System.Drawing.Size(288, 26)
        Me.ComboBoxVersion.TabIndex = 0
        '
        'ChooseLabel
        '
        Me.ChooseLabel.AutoSize = True
        Me.ChooseLabel.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ChooseLabel.Location = New System.Drawing.Point(12, 9)
        Me.ChooseLabel.Name = "ChooseLabel"
        Me.ChooseLabel.Size = New System.Drawing.Size(102, 18)
        Me.ChooseLabel.TabIndex = 1
        Me.ChooseLabel.Text = "Get Elements:"
        '
        'ButtonCancel
        '
        Me.ButtonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.ButtonCancel.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonCancel.Location = New System.Drawing.Point(110, 62)
        Me.ButtonCancel.Name = "ButtonCancel"
        Me.ButtonCancel.Size = New System.Drawing.Size(92, 30)
        Me.ButtonCancel.TabIndex = 2
        Me.ButtonCancel.Text = "&Cancel"
        Me.ButtonCancel.UseVisualStyleBackColor = True
        '
        'ButtonGet
        '
        Me.ButtonGet.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.ButtonGet.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonGet.Location = New System.Drawing.Point(208, 62)
        Me.ButtonGet.Name = "ButtonGet"
        Me.ButtonGet.Size = New System.Drawing.Size(92, 30)
        Me.ButtonGet.TabIndex = 3
        Me.ButtonGet.Text = "&Get"
        Me.ButtonGet.UseVisualStyleBackColor = True
        '
        'ButtonIgnore
        '
        Me.ButtonIgnore.DialogResult = System.Windows.Forms.DialogResult.Ignore
        Me.ButtonIgnore.Font = New System.Drawing.Font("Microsoft Sans Serif", 11.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonIgnore.Location = New System.Drawing.Point(12, 62)
        Me.ButtonIgnore.Name = "ButtonIgnore"
        Me.ButtonIgnore.Size = New System.Drawing.Size(92, 30)
        Me.ButtonIgnore.TabIndex = 4
        Me.ButtonIgnore.Text = "&Ignore"
        Me.ButtonIgnore.UseVisualStyleBackColor = True
        '
        'GetElements
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(312, 104)
        Me.Controls.Add(Me.ButtonIgnore)
        Me.Controls.Add(Me.ButtonGet)
        Me.Controls.Add(Me.ButtonCancel)
        Me.Controls.Add(Me.ChooseLabel)
        Me.Controls.Add(Me.ComboBoxVersion)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MaximizeBox = False
        Me.Name = "GetElements"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Get Elements"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ComboBoxVersion As System.Windows.Forms.ComboBox
    Friend WithEvents ChooseLabel As System.Windows.Forms.Label
    Friend WithEvents ButtonCancel As System.Windows.Forms.Button
    Friend WithEvents ButtonGet As System.Windows.Forms.Button
    Friend WithEvents ButtonIgnore As System.Windows.Forms.Button
End Class
