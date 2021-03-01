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
        Public Image1Index As Integer
        Public Text As String
        Public UserID As Integer
        Public User As String
        Public Image2Index As Integer

        Public Sub New(ByVal _User As String, ByVal _Text As String, ByVal _Image1Index As Integer, ByVal _Image2Index As Integer, ByVal _UserID As Integer)
            User = _User
            Text = _Text
            Image1Index = _Image1Index
            Image2Index = _Image2Index
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

    Public Property SelectedItemEX As Object
        Get
            Return mListBox.SelectedItem()
        End Get
        Set(ByVal value As Object)
            mListBox.SelectedItem() = value
        End Set
    End Property

    Public Sub IndexRemove(ByVal nValue As Integer)
        mListBox.Items.Remove(mListBox.Items.Item(nValue))
    End Sub

    Public Sub Insert(ByVal idx As Integer, ByVal _User As String, ByVal _Text As String, Optional ByVal _Image1Index As Integer = -1, Optional ByVal _Image2Index As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Insert(idx, New ItemEX(_User, _Text, _Image1Index, _Image2Index, _UserID))
    End Sub

    Public Sub Replace(ByVal idx As Integer, ByVal _User As String, ByVal _Text As String, Optional ByVal _Image1Index As Integer = -1, Optional ByVal _Image2Index As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Remove(mListBox.Items.Item(idx))
        mListBox.Items.Insert(idx, New ItemEX(_User, _Text, _Image1Index, _Image2Index, _UserID))
    End Sub

    Public Sub AddImage(ByVal filename As System.Drawing.Image)
        Images.Add(filename)
    End Sub

    Public Sub New()
        MyBase.New()
        Me.DrawMode = DrawMode.OwnerDrawVariable
        mListBox = Me
    End Sub

    Public Sub Add(ByVal _User As String, ByVal _Text As String, Optional ByVal _Image1Index As Integer = -1, Optional ByVal _Image2Index As Integer = -1, Optional ByVal _UserID As Integer = -1)
        mListBox.Items.Add(New ItemEX(_User, _Text, _Image1Index, _Image2Index, _UserID))
    End Sub

    Private Sub mListBox_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles mListBox.Resize
        RaiseEvent Resize(sender, e, 1)
    End Sub

    Private Sub DrawItemHandler(ByVal sender As Object, ByVal e As DrawItemEventArgs) Handles mListBox.DrawItem
        e.DrawBackground()
        e.DrawFocusRectangle()
        Dim titFont As New Font("Tahoma", 18, FontStyle.Bold, GraphicsUnit.Pixel)
        Dim subTitFont As New Font("Tahoma", 12, FontStyle.Regular, GraphicsUnit.Pixel)
        'Dim titFont As New Font("Tahoma", 14, FontStyle.Bold, GraphicsUnit.Pixel)
        'Dim subTitFont As New Font("Tahoma", 10, FontStyle.Regular, GraphicsUnit.Pixel)
        Dim titBrush As New SolidBrush(Color.DimGray)
        Dim subTitBrush As New SolidBrush(Color.Gray)
        Dim idx As Integer = e.Index
        If idx > -1 Then
            e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).Text, titFont, titBrush, e.Bounds.Left + 70, e.Bounds.Top)
            e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).User, subTitFont, subTitBrush, e.Bounds.Left + 70, e.Bounds.Top + titFont.Height + 1)
            'e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).Text, titFont, titBrush, e.Bounds.Left + 56, e.Bounds.Top)
            'e.Graphics.DrawString(DirectCast(mListBox.Items(idx), ItemEX).User, subTitFont, subTitBrush, e.Bounds.Left + 56, e.Bounds.Top + titFont.Height + 2)
            e.Graphics.DrawLine(New Pen(Color.LightGray), 0, e.Bounds.Bottom - 1, e.Bounds.Right, e.Bounds.Bottom - 1)

            If Items(idx).Image1Index > -1 Then
                e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image1Index), Image), e.Bounds.Left + 3, e.Bounds.Top + 5, 28, 28)
                'e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image1Index), Image), e.Bounds.Left + 1, e.Bounds.Top + 3, 32, 32)
                'e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image1Index), Image), e.Bounds.Left + 1, e.Bounds.Top + 2, 24, 24)
            End If

            If Items(idx).Image2Index > -1 Then
                e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image2Index), Image), e.Bounds.Left + 35, e.Bounds.Top + 5, 28, 28)
                'e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image2Index), Image), e.Bounds.Left + 35, e.Bounds.Top + 3, 32, 32)
                'e.Graphics.DrawImage(DirectCast(Images(Items(idx).Image2Index), Image), e.Bounds.Left + 28, e.Bounds.Top + 2, 24, 24)
            End If

        End If
    End Sub

    Private Sub MeasureItemHandler(ByVal sender As Object, ByVal e As MeasureItemEventArgs) Handles mListBox.MeasureItem
        'e.ItemHeight = 80
        Dim a As Integer = CountLine(Items(e.Index).User) + 2
        e.ItemHeight = a * (Font.Height) ' + Margin.Horizontal)
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
End Class
