Option Explicit On
Option Strict Off

Public Class ListBoxEX
    Inherits System.Windows.Forms.ListBox
    Public Shadows Event Resize(ByVal sender As Object, ByVal e As EventArgs, ByVal i As Integer)
    Public Shadows Event DrawItem(ByVal sender As Object, ByVal e As DrawItemEventArgs)
    Public Shadows Event MeasureItem(ByVal sender As Object, ByVal e As MeasureItemEventArgs)
    Public Shadows Event ValueMemberChanged(ByVal sender As Object, ByVal e As EventArgs)
    Public Shadows Event VisibleChanged(ByVal sender As Object, ByVal e As EventArgs)

    Private WithEvents mListBox As System.Windows.Forms.ListBox

    Private Images As New ArrayList

    Public Shadows Function Items(ByVal idx As Integer) As ItemEX
        Return mListBox.Items(idx)
    End Function

    Public Class ItemEX
        Public ImageIndex As Integer
        Public Text As String
        Public UserID As Integer
        Public User As String

        Public Sub New(ByVal _User As String, ByVal _Text As String, ByVal _ImageIndex As Integer, ByVal _UserID As Integer)
            User = _User
            Text = _Text
            ImageIndex = _ImageIndex
            UserID = _UserID
        End Sub
    End Class

    Public Sub Clear()
        mListBox.Items.Clear()
    End Sub

    Public Property IndexItem() As Integer
        Get
            Return mListBox.TopIndex()
        End Get
        Set(ByVal value As Integer)
            mListBox.TopIndex() = value
        End Set

    End Property

    Public Property SelectedIndexItem() As Integer
        Get
            Return mListBox.SelectedIndex()
        End Get
        Set(ByVal value As Integer)
            mListBox.SelectedIndex() = value
        End Set
    End Property


    Public Sub IndexRemove(ByVal nValue As Integer)
        mListBox.Items.Remove(mListBox.Items.Item(nValue))
    End Sub

    Public Sub Insert(ByVal idx As Integer, ByVal _User As String, ByVal _Text As String, Optional ByVal _ImageIndex As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Insert(idx, New ItemEX(_User, _Text, _ImageIndex, _UserID))
    End Sub

    Public Function GetImageIndex(ByVal idx As Integer) As Integer
        Return Items(idx).ImageIndex
    End Function

    Public Sub Replace(ByVal idx As Integer, ByVal _User As String, ByVal _Text As String, Optional ByVal _ImageIndex As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Remove(mListBox.Items.Item(idx))
        mListBox.Items.Insert(idx, New ItemEX(_User, _Text, _ImageIndex, _UserID))
    End Sub

    Public Sub AddIcon(ByVal filename As System.Drawing.Icon)
        Images.Add(filename)
    End Sub

    Public Sub New()
        MyBase.New()
        Me.DrawMode = DrawMode.OwnerDrawVariable
        mListBox = Me
    End Sub

    Public Sub Add(ByVal _User As String, ByVal _Text As String, Optional ByVal _ImageIndex As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Add(New ItemEX(_User, _Text, _ImageIndex, _UserID))
    End Sub

    Private Sub mListBox_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles mListBox.Resize
        RaiseEvent Resize(sender, e, 1)
    End Sub

    Private Sub DrawItemHandler(ByVal sender As Object, ByVal e As DrawItemEventArgs) Handles mListBox.DrawItem
        e.DrawBackground()
        e.DrawFocusRectangle()
        Dim titFont As New Font("Tahoma", 12, FontStyle.Bold, GraphicsUnit.Pixel)
        Dim subTitFont As New Font("Tahoma", 11, FontStyle.Regular, GraphicsUnit.Pixel)
        Dim titBrush As New SolidBrush(Color.DimGray)
        Dim subTitBrush As New SolidBrush(Color.Gray)
        Dim idx As Integer = e.Index
        If idx > -1 Then
            e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).Text, titFont, titBrush, e.Bounds.Left + 30, e.Bounds.Top)
            e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).User, subTitFont, subTitBrush, e.Bounds.Left + 30, e.Bounds.Top + titFont.Height + 2)
            e.Graphics.DrawLine(New Pen(Color.LightGray), 0, e.Bounds.Bottom - 1, e.Bounds.Right, e.Bounds.Bottom - 1)
            If Items(idx).ImageIndex > -1 Then
                e.Graphics.DrawIcon(DirectCast(Images(Items(idx).ImageIndex), Icon), e.Bounds.Left + 1, e.Bounds.Top + 2)

                'Ajout d'un second icon
                'e.Graphics.DrawIcon(DirectCast(Images(Items(idx).ImageIndex), Icon), e.Bounds.Left + 31, e.Bounds.Top + 2)

            End If
        End If
    End Sub

    Private Sub MeasureItemHandler(ByVal sender As Object, ByVal e As MeasureItemEventArgs) Handles mListBox.MeasureItem
        'e.ItemHeight = 80
        Dim a As Integer = CountLine(Items(e.Index).User) + 2
        e.ItemHeight = a * (Font.Height) ' + Margin.Horizontal)
        'HorizontalScrollbar = True
        RaiseEvent MeasureItem(sender, e)
    End Sub

    Private Sub VisibleChangedHandler(ByVal sender As Object, ByVal e As System.EventArgs) Handles mListBox.VisibleChanged
        '***********************************
    End Sub

    Private Sub ValueMemberChangedHandler(ByVal sender As Object, ByVal e As System.EventArgs) Handles mListBox.ValueMemberChanged
        '***********************************
    End Sub

    Private Function CountLine(ByVal svValue As String) As Integer
        Dim nPosNewLine As Integer = InStr(svValue, vbNewLine, CompareMethod.Text)
        CountLine = 1
        While nPosNewLine > 0
            CountLine = CountLine + 1
            nPosNewLine = InStr(nPosNewLine + 1, svValue, vbNewLine, CompareMethod.Text)
        End While
    End Function

    Private Sub InitializeComponent()
        Me.SuspendLayout()
        '
        'ListBoxEX
        '
        Me.Size = New System.Drawing.Size(120, 95)
        Me.ResumeLayout(False)

    End Sub
End Class
