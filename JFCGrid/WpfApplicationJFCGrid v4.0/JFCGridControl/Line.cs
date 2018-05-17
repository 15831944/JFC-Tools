using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace JFCGridControl
{
    //public class Line : FrameworkElement, INotifyPropertyChangedExtended<Object>
    //{
    //    private VisualCollection _children;
    //    int childrenCount = 0;

    //    //Size LineSize = new Size(0, 0);

    //    public int Thickness
    //    {
    //        get { return (int)GetValue(ThicknessProperty); }
    //        set { SetValue(ThicknessProperty, value); }
    //    }

    //    // Using a DependencyProperty as the backing store for Thickness.  This enables animation, styling, binding, etc...
    //    public static readonly DependencyProperty ThicknessProperty =
    //        DependencyProperty.Register("Thickness", typeof(int), typeof(Line), new UIPropertyMetadata(1, new PropertyChangedCallback(UpdateView)));

    //    public Orientation Orientation
    //    {
    //        get { return (Orientation)GetValue(OrientationProperty); }
    //        set { SetValue(OrientationProperty, value); }
    //    }

    //    // Using a DependencyProperty as the backing store for Orientation.  This enables animation, styling, binding, etc...
    //    public static readonly DependencyProperty OrientationProperty =
    //        DependencyProperty.Register("Orientation", typeof(Orientation), typeof(Line), new UIPropertyMetadata(Orientation.Horizontal, new PropertyChangedCallback(UpdateView)));

    //    public Brush LineBrush
    //    {
    //        get { return (Brush)GetValue(LineBrushProperty); }
    //        set { SetValue(LineBrushProperty, value); }
    //    }

    //    // Using a DependencyProperty as the backing store for LineBrush.  This enables animation, styling, binding, etc...
    //    public static readonly DependencyProperty LineBrushProperty =
    //        DependencyProperty.Register("LineBrush", typeof(Brush), typeof(Line), new UIPropertyMetadata(Brushes.Black, new PropertyChangedCallback(UpdateView)));


    //    public Line()
    //    {
    //        this.ClipToBounds = true;
    //        this.Focusable = false;

    //        _children = new VisualCollection(this);

    //        this.SizeChanged += new SizeChangedEventHandler(Line_SizeChanged);
    //    }

    //    public Line(Orientation orientation):this()
    //    {
    //        this.Orientation = orientation;            
    //    }

    //    void Line_SizeChanged(object sender, SizeChangedEventArgs e)
    //    {
    //        Line line = sender as Line;

    //        line.childrenCount = 0;
    //        line._children.Clear();
    //        line._children.Add(line.CreateDrawingVisualText());
    //        line.childrenCount = 1;

    //        line.InvalidateMeasure();
    //    }

    //    private static void UpdateView(DependencyObject obj, DependencyPropertyChangedEventArgs e)
    //    {
    //        Line l = obj as Line;

    //        if (e.NewValue != e.OldValue)
    //        {
    //            //l.LineSize = new Size();

    //            l.childrenCount = 0;
    //            l._children.Clear();
    //            l._children.Add(l.CreateDrawingVisualText());
    //            l.childrenCount = 1;

    //            l.InvalidateMeasure();
    //        }
    //    }

    //    private DrawingVisual CreateDrawingVisualText()
    //    {
    //        if (this.Thickness == 0)
    //            return null;

    //        if (this.ActualWidth == 0 || this.ActualHeight == 0)
    //            return null;

    //        // Create an instance of a DrawingVisual.
    //        DrawingVisual drawingVisual = new DrawingVisual();

    //        // Retrieve the DrawingContext from the DrawingVisual.
    //        DrawingContext drawingContext = drawingVisual.RenderOpen();

    //        Point pointStart;
    //        Point pointEnd;

    //        if (this.Orientation == Orientation.Horizontal)
    //        {
    //            pointStart = new Point(0, (double)this.Thickness / 2);
    //            pointEnd = new Point(this.ActualWidth, (double)this.Thickness / 2);
    //            this.Height = this.Thickness;
    //        }
    //        else
    //        {
    //            pointStart = new Point(this.Thickness / 2, 0);
    //            pointEnd = new Point(this.Thickness / 2, this.ActualHeight);
    //            this.Width = this.Thickness;
    //        }


    //        Pen pen = new Pen(this.LineBrush, this.Thickness);
    //        drawingContext.DrawLine(pen, pointStart, pointEnd);

    //        // Close the DrawingContext to persist changes to the DrawingVisual.
    //        drawingContext.Close();

    //        return drawingVisual;
    //    }

    //    protected override Size MeasureOverride(Size availableSize)
    //    {
    //        Size sTmp;

    //        if (this.Orientation == Orientation.Horizontal)
    //        {
    //            sTmp = new Size(this.ActualWidth, this.Thickness);
    //        }
    //        else
    //        {
    //            sTmp = new Size(this.Thickness, this.ActualHeight);
    //        }

    //        return sTmp;
    //    }

    //    // Provide a required override for the VisualChildrenCount property.
    //    protected override int VisualChildrenCount
    //    {
    //        //get { return 0; }
    //        //get { return _children.Count; }
    //        get { return childrenCount; }
    //    }

    //    // Provide a required override for the GetVisualChild method.
    //    protected override Visual GetVisualChild(int index)
    //    {
    //        //if (index < 0 || index > _children.Count)
    //        if (index < 0 || index > childrenCount)
    //        {
    //            throw new ArgumentOutOfRangeException();
    //        }

    //        return _children[index];
    //    }

    //    //
    //    public event PropertyChangedExtendedEventHandler<Object> PropertyChanged;
    //    internal void OnPropertyChanged(PropertyChangedExtendedEventArgs<Object> e)
    //    {
    //        if (PropertyChanged != null)
    //            PropertyChanged(this, e);
    //    }

    //    public void ResetEvent()
    //    {
    //        PropertyChanged = null;
    //    }

    //}
}
