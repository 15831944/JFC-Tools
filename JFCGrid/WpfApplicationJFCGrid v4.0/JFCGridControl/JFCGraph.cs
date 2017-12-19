// JFCGridControl.JFCGraph
using JFCGridControl;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;


namespace JFCGridControl
{
    public class JFCGraph : FrameworkElement, INotifyPropertyChanged
    {
        public enum GraphTypes
        {
            Curve,
            CurveStart0,
            Histogram
        }

        public class Serie
        {
            public class Value
            {
                public double Point;

                public string Label;
            }

            public string Name;

            public Brush Color;

            public bool Selected;

            public IEnumerable<Value> Points;
        }

        public static readonly DependencyProperty GraphTypeProperty = DependencyProperty.Register("GraphType", typeof(GraphTypes), typeof(JFCGraph), new UIPropertyMetadata(GraphTypes.Histogram, JFCGraph.UpdateGraph));

        public static readonly DependencyProperty SeriesProperty = DependencyProperty.Register("Series", typeof(List<Serie>), typeof(JFCGraph), new UIPropertyMetadata(new List<Serie>(), JFCGraph.UpdateGraph));

        public static readonly DependencyProperty BarSizeProperty = DependencyProperty.Register("BarSize", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(10));

        public static readonly DependencyProperty BeforeSpaceBarProperty = DependencyProperty.Register("BeforeSpaceBar", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(0));

        public static readonly DependencyProperty AfterSpaceBarProperty = DependencyProperty.Register("AfterSpaceBar", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(0));

        public static readonly DependencyProperty BackgroundProperty = DependencyProperty.Register("Background", typeof(Brush), typeof(JFCGraph), new UIPropertyMetadata(Brushes.White, JFCGraph.UpdateGraph));

        public static readonly DependencyProperty SizeRectPointProperty = DependencyProperty.Register("SizeRectPoint", typeof(Size), typeof(JFCGraph), new UIPropertyMetadata(default(Size), JFCGraph.UpdateGraph));

        private VisualCollection _children;

        private int childrenCount;

        private Size GraphSize = new Size(0.0, 0.0);

        private Serie serieOver;

        private int ptOver;

        public GraphTypes GraphType
        {
            get
            {
                return (GraphTypes)base.GetValue(JFCGraph.GraphTypeProperty);
            }
            set
            {
                base.SetValue(JFCGraph.GraphTypeProperty, value);
            }
        }

        public List<Serie> Series
        {
            get
            {
                return (List<Serie>)base.GetValue(JFCGraph.SeriesProperty);
            }
            set
            {
                base.SetValue(JFCGraph.SeriesProperty, value);
            }
        }

        public int BarSize
        {
            get
            {
                return (int)base.GetValue(JFCGraph.BarSizeProperty);
            }
            set
            {
                base.SetValue(JFCGraph.BarSizeProperty, value);
            }
        }

        public int BeforeSpaceBar
        {
            get
            {
                return (int)base.GetValue(JFCGraph.BeforeSpaceBarProperty);
            }
            set
            {
                base.SetValue(JFCGraph.BeforeSpaceBarProperty, value);
            }
        }

        public int AfterSpaceBar
        {
            get
            {
                return (int)base.GetValue(JFCGraph.AfterSpaceBarProperty);
            }
            set
            {
                base.SetValue(JFCGraph.AfterSpaceBarProperty, value);
            }
        }

        public Brush Background
        {
            get
            {
                return (Brush)base.GetValue(JFCGraph.BackgroundProperty);
            }
            set
            {
                base.SetValue(JFCGraph.BackgroundProperty, value);
            }
        }

        public Size SizeRectPoint
        {
            get
            {
                return (Size)base.GetValue(JFCGraph.SizeRectPointProperty);
            }
            set
            {
                base.SetValue(JFCGraph.SizeRectPointProperty, value);
            }
        }

        protected override int VisualChildrenCount
        {
            get
            {
                return this.childrenCount;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public JFCGraph()
        {
            this._children = new VisualCollection(this);
            base.SizeChanged += this.JFCGraph_SizeChanged;
            base.SetValue(RenderOptions.ClearTypeHintProperty, ClearTypeHint.Enabled);
            base.MouseMove += this.JFCGraph_MouseMove;
            base.MouseLeave += this.JFCGraph_MouseLeave;
        }

        public void PointHilite(Serie serieHilite, int idxPoint)
        {
            bool flag = false;
            if (this.serieOver != serieHilite)
            {
                this.serieOver = serieHilite;
                flag = true;
            }
            if (this.ptOver != idxPoint)
            {
                this.ptOver = idxPoint;
                flag = true;
            }
            if (flag)
            {
                JFCGraph.UpdateGraph(this, default(DependencyPropertyChangedEventArgs));
            }
        }

        private void JFCGraph_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.Series != null)
            {
                Point position = e.GetPosition(this);
                bool flag = false;
                Serie serie = null;
                double num = 0.0;
                int num2 = 0;
                List<Serie>.Enumerator enumerator;
                if (this.GraphType == GraphTypes.Curve)
                {
                    int num3 = (int)(position.X - position.X % (double)this.BarSize) / this.BarSize;
                    enumerator = this.Series.GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            Serie current = enumerator.Current;
                            if (current.Points.Count() > num3)
                            {
                                double point = current.Points.ElementAt(num3).Point;
                                double y = position.Y;
                                double num4 = base.ActualHeight - point;
                                Size sizeRectPoint = this.SizeRectPoint;
                                if (y >= num4 - sizeRectPoint.Height / 2.0)
                                {
                                    double y2 = position.Y;
                                    double num5 = base.ActualHeight - point;
                                    sizeRectPoint = this.SizeRectPoint;
                                    if (y2 <= num5 + sizeRectPoint.Height / 2.0)
                                    {
                                        double x = position.X;
                                        double num6 = (double)(num3 * this.BarSize + this.BarSize / 2);
                                        sizeRectPoint = this.SizeRectPoint;
                                        if (x >= num6 - sizeRectPoint.Width / 2.0)
                                        {
                                            double x2 = position.X;
                                            double num7 = (double)(num3 * this.BarSize + this.BarSize / 2);
                                            sizeRectPoint = this.SizeRectPoint;
                                            if (x2 <= num7 + sizeRectPoint.Width / 2.0)
                                            {
                                                num = base.ActualHeight - point;
                                                num2 = num3;
                                                serie = current;
                                                flag = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
                else if (this.GraphType == GraphTypes.CurveStart0)
                {
                    int num8 = (int)(position.X - position.X % (double)this.BarSize) / this.BarSize;
                    enumerator = this.Series.GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            Serie current2 = enumerator.Current;
                            if (current2.Points.Count() > num8)
                            {
                                double point2 = current2.Points.ElementAt(num8).Point;
                                if (position.Y >= base.ActualHeight - point2 && num < base.ActualHeight - point2)
                                {
                                    num = base.ActualHeight - point2;
                                    num2 = num8;
                                    serie = current2;
                                    flag = true;
                                }
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
                else if (this.GraphType == GraphTypes.Histogram)
                {
                    int num9 = (int)(position.X - position.X % (double)this.BarSize) / this.BarSize;
                    enumerator = this.Series.GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            Serie current3 = enumerator.Current;
                            if (current3.Points.Count() > num9)
                            {
                                double point3 = current3.Points.ElementAt(num9).Point;
                                if (position.Y >= base.ActualHeight - point3 && num < base.ActualHeight - point3)
                                {
                                    num = base.ActualHeight - point3;
                                    num2 = num9;
                                    serie = current3;
                                    flag = true;
                                }
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
                if (!flag && this.serieOver != null)
                {
                    flag = true;
                }
                if (flag)
                {
                    this.serieOver = serie;
                    this.ptOver = num2;
                    JFCGraph.UpdateGraph(this, default(DependencyPropertyChangedEventArgs));
                }
            }
        }

        private void JFCGraph_MouseLeave(object sender, MouseEventArgs e)
        {
            this.serieOver = null;
            this.ptOver = 0;
            JFCGraph.UpdateGraph(this, default(DependencyPropertyChangedEventArgs));
        }

        public void Refresh()
        {
            this.childrenCount = 0;
            this._children.Clear();
            this._children.Add(this.CreateDrawingVisualGraph());
            this.childrenCount = 1;
            base.InvalidateMeasure();
        }

        private void JFCGraph_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            JFCGraph jFCGraph = sender as JFCGraph;
            jFCGraph.childrenCount = 0;
            jFCGraph._children.Clear();
            jFCGraph._children.Add(jFCGraph.CreateDrawingVisualGraph());
            jFCGraph.childrenCount = 1;
            jFCGraph.InvalidateMeasure();
        }

        private static void UpdateGraph(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGraph jFCGraph = obj as JFCGraph;
            if (e.NewValue == e.OldValue && e.Property != null)
            {
                return;
            }
            jFCGraph.GraphSize = default(Size);
            jFCGraph.childrenCount = 0;
            jFCGraph._children.Clear();
            jFCGraph._children.Add(jFCGraph.CreateDrawingVisualGraph());
            jFCGraph.childrenCount = 1;
            jFCGraph.InvalidateMeasure();
        }

        private DrawingVisual CreateDrawingVisualGraph()
        {
            if (base.ActualWidth != 0.0 && base.ActualHeight != 0.0)
            {
                DrawingVisual drawingVisual = new DrawingVisual();
                DrawingContext drawingContext = drawingVisual.RenderOpen();
                drawingContext.DrawRectangle(this.Background, null, new Rect(new Size(base.ActualWidth, base.ActualHeight)));
                Point point = default(Point);
                int num = 0;
                if (this.Series != null)
                {
                    foreach (Serie item in this.Series)
                    {
                        Point point2 = new Point(0.0, base.ActualHeight);
                        Pen pen = new Pen();
                        pen.Brush = item.Color;
                        pen.Thickness = 1.0;
                        Brush color = item.Color;
                        if (item.Color is SolidColorBrush)
                        {
                            SolidColorBrush solidColorBrush = item.Color as SolidColorBrush;
                            Color color2 = default(Color);
                            color2.A = 128;
                            Color color3 = solidColorBrush.Color;
                            color2.R = color3.R;
                            color3 = solidColorBrush.Color;
                            color2.G = color3.G;
                            color3 = solidColorBrush.Color;
                            color2.B = color3.B;
                            color = new SolidColorBrush(color2);
                        }
                        else
                        {
                            color = item.Color;
                        }
                        int num2 = 0;
                        if (item.Points != null)
                        {
                            int num3 = item.Points.Count();
                            if (num < num3)
                            {
                                num = num3;
                            }
                            foreach (Serie.Value point3 in item.Points)
                            {
                                Size sizeRectPoint;
                                if (this.GraphType == GraphTypes.Curve)
                                {
                                    point.X = (double)(num2 * this.BarSize + this.BarSize / 2);
                                    point.Y = base.ActualHeight - point3.Point;
                                    if (num2 > 0)
                                    {
                                        drawingContext.DrawLine(pen, point2, point);
                                        if (num2 == 1)
                                        {
                                            Size sizeRectPoint2 = this.SizeRectPoint;
                                            sizeRectPoint = this.SizeRectPoint;
                                            if (sizeRectPoint.Width != 0.0)
                                            {
                                                sizeRectPoint = this.SizeRectPoint;
                                                if (sizeRectPoint.Height != 0.0)
                                                {
                                                    DrawingContext drawingContext2 = drawingContext;
                                                    Brush color4 = item.Color;
                                                    Pen pen2 = pen;
                                                    double x = point2.X;
                                                    sizeRectPoint = this.SizeRectPoint;
                                                    double x2 = x - sizeRectPoint.Width / 2.0;
                                                    double y = point2.Y;
                                                    sizeRectPoint = this.SizeRectPoint;
                                                    drawingContext2.DrawRectangle(color4, pen2, new Rect(new Point(x2, y - sizeRectPoint.Height / 2.0), this.SizeRectPoint));
                                                }
                                            }
                                        }
                                        Size sizeRectPoint3 = this.SizeRectPoint;
                                        sizeRectPoint = this.SizeRectPoint;
                                        if (sizeRectPoint.Width != 0.0)
                                        {
                                            sizeRectPoint = this.SizeRectPoint;
                                            if (sizeRectPoint.Height != 0.0)
                                            {
                                                DrawingContext drawingContext3 = drawingContext;
                                                Brush color5 = item.Color;
                                                Pen pen3 = pen;
                                                double x3 = point.X;
                                                sizeRectPoint = this.SizeRectPoint;
                                                double x4 = x3 - sizeRectPoint.Width / 2.0;
                                                double y2 = point.Y;
                                                sizeRectPoint = this.SizeRectPoint;
                                                drawingContext3.DrawRectangle(color5, pen3, new Rect(new Point(x4, y2 - sizeRectPoint.Height / 2.0), this.SizeRectPoint));
                                            }
                                        }
                                        if (item.Selected)
                                        {
                                            List<Point> list = new List<Point>();
                                            list.Add(new Point(point2.X, base.ActualHeight));
                                            list.Add(point2);
                                            list.Add(point);
                                            list.Add(new Point(point.X, base.ActualHeight));
                                            list.Add(new Point(point2.X, base.ActualHeight));
                                            PathGeometry pathGeometry = new PathGeometry();
                                            PathFigure pathFigure = new PathFigure();
                                            pathFigure.StartPoint = list[0];
                                            list.Remove(list[0]);
                                            pathFigure.Segments.Add(new PolyLineSegment(list, false));
                                            pathGeometry.Figures.Add(pathFigure);
                                            drawingContext.DrawGeometry(color, null, pathGeometry);
                                        }
                                    }
                                    else if (item.Points.Count() == 1)
                                    {
                                        Size sizeRectPoint4 = this.SizeRectPoint;
                                        sizeRectPoint = this.SizeRectPoint;
                                        if (sizeRectPoint.Width != 0.0)
                                        {
                                            sizeRectPoint = this.SizeRectPoint;
                                            if (sizeRectPoint.Height != 0.0)
                                            {
                                                DrawingContext drawingContext4 = drawingContext;
                                                Brush color6 = item.Color;
                                                Pen pen4 = pen;
                                                double x5 = point.X;
                                                sizeRectPoint = this.SizeRectPoint;
                                                double x6 = x5 - sizeRectPoint.Width / 2.0;
                                                double y3 = point.Y;
                                                sizeRectPoint = this.SizeRectPoint;
                                                drawingContext4.DrawRectangle(color6, pen4, new Rect(new Point(x6, y3 - sizeRectPoint.Height / 2.0), this.SizeRectPoint));
                                            }
                                        }
                                    }
                                    if (this.serieOver == item && this.ptOver == num2)
                                    {
                                        FormattedText formattedText = new FormattedText(point3.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, Brushes.Black);
                                        double x7 = point.X - formattedText.Height / 2.0;
                                        double y4 = point.Y;
                                        sizeRectPoint = this.SizeRectPoint;
                                        Point origin = new Point(x7, y4 - sizeRectPoint.Height / 2.0 - 5.0);
                                        RotateTransform rotateTransform = new RotateTransform();
                                        rotateTransform.CenterX = origin.X;
                                        rotateTransform.CenterY = origin.Y;
                                        rotateTransform.Angle = -90.0;
                                        drawingContext.PushTransform(rotateTransform);
                                        drawingContext.DrawText(formattedText, origin);
                                        drawingContext.Pop();
                                    }
                                    point2.X = point.X;
                                    point2.Y = point.Y;
                                }
                                else if (this.GraphType == GraphTypes.CurveStart0)
                                {
                                    point.X = (double)((num2 + 1) * this.BarSize);
                                    point.Y = base.ActualHeight - point3.Point;
                                    drawingContext.DrawLine(pen, point2, point);
                                    Size sizeRectPoint5 = this.SizeRectPoint;
                                    sizeRectPoint = this.SizeRectPoint;
                                    if (sizeRectPoint.Width != 0.0)
                                    {
                                        sizeRectPoint = this.SizeRectPoint;
                                        if (sizeRectPoint.Height != 0.0)
                                        {
                                            DrawingContext drawingContext5 = drawingContext;
                                            Brush color7 = item.Color;
                                            Pen pen5 = pen;
                                            double x8 = point.X;
                                            sizeRectPoint = this.SizeRectPoint;
                                            double x9 = x8 - sizeRectPoint.Width / 2.0;
                                            double y5 = point.Y;
                                            sizeRectPoint = this.SizeRectPoint;
                                            drawingContext5.DrawRectangle(color7, pen5, new Rect(new Point(x9, y5 - sizeRectPoint.Height / 2.0), this.SizeRectPoint));
                                        }
                                    }
                                    if (item.Selected)
                                    {
                                        List<Point> list2 = new List<Point>();
                                        list2.Add(new Point(point2.X, base.ActualHeight));
                                        list2.Add(point2);
                                        list2.Add(point);
                                        list2.Add(new Point(point.X, base.ActualHeight));
                                        list2.Add(new Point(point2.X, base.ActualHeight));
                                        PathGeometry pathGeometry2 = new PathGeometry();
                                        PathFigure pathFigure2 = new PathFigure();
                                        pathFigure2.StartPoint = list2[0];
                                        list2.Remove(list2[0]);
                                        pathFigure2.Segments.Add(new PolyLineSegment(list2, false));
                                        pathGeometry2.Figures.Add(pathFigure2);
                                        drawingContext.DrawGeometry(color, null, pathGeometry2);
                                    }
                                    if (this.serieOver == item && this.ptOver == num2)
                                    {
                                        List<Point> list3 = new List<Point>();
                                        list3.Add(new Point(point2.X, base.ActualHeight));
                                        list3.Add(point2);
                                        list3.Add(point);
                                        list3.Add(new Point(point.X, base.ActualHeight));
                                        list3.Add(new Point(point2.X, base.ActualHeight));
                                        PathGeometry pathGeometry3 = new PathGeometry();
                                        PathFigure pathFigure3 = new PathFigure();
                                        pathFigure3.StartPoint = list3[0];
                                        list3.Remove(list3[0]);
                                        pathFigure3.Segments.Add(new PolyLineSegment(list3, false));
                                        pathGeometry3.Figures.Add(pathFigure3);
                                        drawingContext.DrawGeometry(color, null, pathGeometry3);
                                        FormattedText formattedText2 = new FormattedText(point3.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, Brushes.Black);
                                        Point origin2 = new Point(point2.X - (formattedText2.Height - (double)this.BarSize) / 2.0, point.Y - 5.0);
                                        RotateTransform rotateTransform2 = new RotateTransform();
                                        rotateTransform2.CenterX = origin2.X;
                                        rotateTransform2.CenterY = origin2.Y;
                                        rotateTransform2.Angle = -90.0;
                                        drawingContext.PushTransform(rotateTransform2);
                                        drawingContext.DrawText(formattedText2, origin2);
                                        drawingContext.Pop();
                                    }
                                    point2.X = point.X;
                                    point2.Y = point.Y;
                                }
                                else if (this.GraphType == GraphTypes.Histogram)
                                {
                                    point.X = (double)((num2 + 1) * this.BarSize);
                                    point.Y = base.ActualHeight - point3.Point;
                                    if (point3.Point > 0.0)
                                    {
                                        int num4 = num2 * this.BarSize + this.BeforeSpaceBar;
                                        int num5 = (int)base.ActualHeight - (int)point3.Point;
                                        int num6 = this.BarSize - this.BeforeSpaceBar - this.AfterSpaceBar;
                                        int num7 = (int)point3.Point - 1;
                                        if (num6 < 0)
                                        {
                                            num6 = 0;
                                        }
                                        if (num7 < 0)
                                        {
                                            num7 = 0;
                                        }
                                        double num8 = 0.0;
                                        if (pen.Thickness % 2.0 != 0.0)
                                        {
                                            num8 = 0.5;
                                        }
                                        Brush brush = null;
                                        if (item.Selected)
                                        {
                                            brush = color;
                                        }
                                        if (this.serieOver == item)
                                        {
                                            if (this.ptOver == num2)
                                            {
                                                brush = color;
                                                drawingContext.DrawRectangle(brush, pen, new Rect((double)num4 + num8, (double)num5 + num8, (double)num6, (double)num7));
                                                FormattedText formattedText3 = new FormattedText(point3.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, Brushes.Black);
                                                Point origin3 = new Point((double)num4 - (formattedText3.Height - (double)this.BarSize) / 2.0, (double)(num5 - 5));
                                                RotateTransform rotateTransform3 = new RotateTransform();
                                                rotateTransform3.CenterX = origin3.X;
                                                rotateTransform3.CenterY = origin3.Y;
                                                rotateTransform3.Angle = -90.0;
                                                drawingContext.PushTransform(rotateTransform3);
                                                drawingContext.DrawText(formattedText3, origin3);
                                                drawingContext.Pop();
                                            }
                                            else
                                            {
                                                drawingContext.DrawRectangle(brush, pen, new Rect((double)num4 + num8, (double)num5 + num8, (double)num6, (double)num7));
                                            }
                                        }
                                        else
                                        {
                                            drawingContext.DrawRectangle(brush, pen, new Rect((double)num4 + num8, (double)num5 + num8, (double)num6, (double)num7));
                                        }
                                        point2.X = (double)num4;
                                        point2.Y = (double)num5;
                                    }
                                }
                                num2++;
                            }
                        }
                    }
                }
                if (this.GraphSize.Width < point.X)
                {
                    this.GraphSize.Width = (double)(num * this.BarSize);
                }
                drawingContext.Close();
                return drawingVisual;
            }
            return null;
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (this.GraphSize.Width == 0.0 && this.GraphSize.Height == 0.0)
            {
                if (this.Series != null)
                {
                    foreach (Serie item in this.Series)
                    {
                        if (item.Points != null)
                        {
                            int num = 0;
                            foreach (Serie.Value point in item.Points)
                            {
                                if (this.GraphSize.Width < (double)(num * this.BarSize))
                                {
                                    this.GraphSize.Width = (double)(num * this.BarSize);
                                }
                                if (this.GraphSize.Height < point.Point)
                                {
                                    this.GraphSize.Height = point.Point;
                                }
                                num++;
                            }
                        }
                    }
                }
                return this.GraphSize;
            }
            return this.GraphSize;
        }

        protected override Visual GetVisualChild(int index)
        {
            if (index >= 0 && index <= this.childrenCount)
            {
                return this._children[index];
            }
            throw new ArgumentOutOfRangeException();
        }

        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }

        public void ResetEvent()
        {
            this.PropertyChanged = null;
        }
    }
}