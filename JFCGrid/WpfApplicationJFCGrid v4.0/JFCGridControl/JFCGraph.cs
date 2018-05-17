using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.ComponentModel;
using System.Windows.Media;
using System.Globalization;

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

        public GraphTypes GraphType
        {
            get { return (GraphTypes)GetValue(GraphTypeProperty); }
            set { SetValue(GraphTypeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for GraphType.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GraphTypeProperty =
            DependencyProperty.Register("GraphType", typeof(GraphTypes), typeof(JFCGraph), new UIPropertyMetadata(GraphTypes.Histogram, new PropertyChangedCallback(UpdateGraph)));

        public List<Serie> Series
        {
            get { return (List<Serie>)GetValue(SeriesProperty); }
            set { SetValue(SeriesProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Series.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SeriesProperty =
            DependencyProperty.Register("Series", typeof(List<Serie>), typeof(JFCGraph), new UIPropertyMetadata(new List<Serie>(), new PropertyChangedCallback(UpdateGraph)));

        public int BarSize
        {
            get { return (int)GetValue(BarSizeProperty); }
            set { SetValue(BarSizeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for BarSize.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BarSizeProperty =
            DependencyProperty.Register("BarSize", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(10));

        public int BeforeSpaceBar
        {
            get { return (int)GetValue(BeforeSpaceBarProperty); }
            set { SetValue(BeforeSpaceBarProperty, value); }
        }

        // Using a DependencyProperty as the backing store for BeforeSpaceBar.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BeforeSpaceBarProperty =
            DependencyProperty.Register("BeforeSpaceBar", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(0));

        public int AfterSpaceBar
        {
            get { return (int)GetValue(AfterSpaceBarProperty); }
            set { SetValue(AfterSpaceBarProperty, value); }
        }

        // Using a DependencyProperty as the backing store for AfterSpaceBar.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty AfterSpaceBarProperty =
            DependencyProperty.Register("AfterSpaceBar", typeof(int), typeof(JFCGraph), new UIPropertyMetadata(0));

        public Brush Background
        {
            get { return (Brush)GetValue(BackgroundProperty); }
            set { SetValue(BackgroundProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Background.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BackgroundProperty =
            DependencyProperty.Register("Background", typeof(Brush), typeof(JFCGraph), new UIPropertyMetadata(Brushes.White, new PropertyChangedCallback(UpdateGraph)));

        //public Size SizeRectPoint
        //{
        //    get { return (Size)GetValue(SizeRectPointProperty); }
        //    set { SetValue(SizeRectPointProperty, value); }
        //}

        //// Using a DependencyProperty as the backing store for CurveWithPoint.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty SizeRectPointProperty =
        //    DependencyProperty.Register("SizeRectPoint", typeof(Size), typeof(JFCGraph), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateGraph)));



        public Size SizeRectPoint
        {
            get { return (Size)GetValue(SizeRectPointProperty); }
            set { SetValue(SizeRectPointProperty, value); }
        }

        // Using a DependencyProperty as the backing store for SizeRectPoint.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SizeRectPointProperty =
            DependencyProperty.Register("SizeRectPoint", typeof(Size), typeof(JFCGraph), new UIPropertyMetadata(new Size(), new PropertyChangedCallback(UpdateGraph)));


        // Create a collection of child visual objects.
        private VisualCollection _children;
        int childrenCount = 0;

        Size GraphSize = new Size(0, 0);

        public JFCGraph()
        {
            //this.ClipToBounds = true;
            //this.Focusable = false;

            //this.SnapsToDevicePixels = false;

            _children = new VisualCollection(this);

            this.SizeChanged += new SizeChangedEventHandler(JFCGraph_SizeChanged);

            //this.UseLayoutRounding = false;

            this.SetValue(RenderOptions.ClearTypeHintProperty, ClearTypeHint.Enabled);


            this.MouseMove += new System.Windows.Input.MouseEventHandler(JFCGraph_MouseMove);
            this.MouseLeave += new System.Windows.Input.MouseEventHandler(JFCGraph_MouseLeave);

        }

        Serie serieOver = null;
        int ptOver = 0;

        public void PointHilite(Serie serieHilite, int idxPoint)
        {
            bool update = false;

            if (this.serieOver != serieHilite)
            {
                this.serieOver = serieHilite;
                update = true;
            }

            if (this.ptOver != idxPoint)
            {
                this.ptOver = idxPoint;
                update = true;
            }

            if (update)
            {
                DependencyPropertyChangedEventArgs args = new DependencyPropertyChangedEventArgs();

                UpdateGraph(this, args);
            }
        }

        void JFCGraph_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (Series != null)
            {
                Point pt = e.GetPosition(this);

                Boolean update = false;

                Serie serie = null;
                Double valY = 0;
                int valX = 0;

                if (GraphType == GraphTypes.Curve)
                {
                    int x = (int)(pt.X - (pt.X % BarSize)) / BarSize;

                    foreach (var s in Series)
                    {
                        if (s.Points.Count() > x)
                        {
                            var item = s.Points.ElementAt(x);
                            double y = item.Point;

                            //ptEnd.X = x * BarSize + BarSize / 2;
                            //ptEnd.Y = this.ActualHeight - y;
                            

                            if (pt.Y >= this.ActualHeight - y - SizeRectPoint.Height / 2 && pt.Y <= this.ActualHeight - y + SizeRectPoint.Height / 2)
                            {
                                if (pt.X >= x * BarSize + BarSize / 2 - SizeRectPoint.Width / 2 && pt.X <= x * BarSize + BarSize / 2 + SizeRectPoint.Width / 2)
                                {
                                    valY = this.ActualHeight - y;
                                    valX = x;
                                    serie = s;

                                    update = true;
                                }
                            }
                        }
                    }
                }
                else if (GraphType == GraphTypes.CurveStart0)
                {
                    int x = (int)(pt.X - (pt.X % BarSize)) / BarSize;

                    //x = x + 1;

                    foreach (var s in Series)
                    {
                        if (s.Points.Count() > x)
                        {
                            var item = s.Points.ElementAt(x);
                            double y = item.Point;

                            if (pt.Y >= this.ActualHeight - y)
                            {
                                if (valY < this.ActualHeight - y)
                                {
                                    valY = this.ActualHeight - y;
                                    valX = x;
                                    serie = s;

                                    update = true;
                                }
                            }
                        }
                    }
                }
                else if (GraphType == GraphTypes.Histogram)
                {
                    int x = (int)(pt.X - (pt.X % BarSize)) / BarSize;

                    foreach (var s in Series)
                    {
                        if (s.Points.Count() > x)
                        {
                            var item = s.Points.ElementAt(x);
                            double y = item.Point;

                            if (pt.Y >= this.ActualHeight - y)
                            {
                                if (valY < this.ActualHeight - y)
                                {
                                    valY = this.ActualHeight - y;
                                    valX = x;
                                    serie = s;

                                    update = true;
                                }
                            }
                        }
                    }
                }

                if (update == false && serieOver != null)
                    update = true;

                if (update)
                {
                    serieOver = serie;
                    ptOver = valX;

                    DependencyPropertyChangedEventArgs args = new DependencyPropertyChangedEventArgs();

                    UpdateGraph(this, args);
                }
            }
        }

        void JFCGraph_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            serieOver = null;
            ptOver = 0;

            DependencyPropertyChangedEventArgs args = new DependencyPropertyChangedEventArgs();

            UpdateGraph(this, args);
        }

        public void Refresh()
        {
            childrenCount = 0;
            _children.Clear();
            _children.Add(CreateDrawingVisualGraph());
            childrenCount = 1;

            InvalidateMeasure();
        }

        void JFCGraph_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            JFCGraph graph = sender as JFCGraph;

            graph.childrenCount = 0;
            graph._children.Clear();
            graph._children.Add(graph.CreateDrawingVisualGraph());
            graph.childrenCount = 1;

            graph.InvalidateMeasure();
        }

        private static void UpdateGraph(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGraph graph = obj as JFCGraph;

            if (e.NewValue != e.OldValue || e.Property == null)
            {
                graph.GraphSize = new Size();

                graph.childrenCount = 0;
                graph._children.Clear();
                graph._children.Add(graph.CreateDrawingVisualGraph());
                graph.childrenCount = 1;

                graph.InvalidateMeasure();
            }
        }

        private DrawingVisual CreateDrawingVisualGraph()
        {
            if (this.ActualWidth == 0 || this.ActualHeight == 0)
                return null;

            // Create an instance of a DrawingVisual.
            DrawingVisual drawingVisual = new DrawingVisual();

            // Retrieve the DrawingContext from the DrawingVisual.
            DrawingContext drawingContext = drawingVisual.RenderOpen();

            // On colorie le fond du control
            drawingContext.DrawRectangle(this.Background, null, new Rect(new Size(ActualWidth, ActualHeight)));

            Point pt; // = new Point(0, this.ActualHeight);
            Point ptEnd = new Point();

            int nbPtsMax = 0;

            if (Series != null)
            {
                foreach (var s in Series)
                {
                    pt = new Point(0, this.ActualHeight);

                    Pen pen = new Pen(); // = new Pen(Brushes.Red, 1);
                    pen.Brush = s.Color;

                    //if (s.Selected == true)
                    //    pen.Thickness = 2;
                    //else
                    pen.Thickness = 1;


                    Brush brush = s.Color;

                    //if (GraphType == GraphTypes.Histogram)
                    //{
                    //    if (s.Selected == true)
                    //    {

                    if (s.Color is SolidColorBrush)
                    {
                        var sb = s.Color as SolidColorBrush;

                        Color color = new Color();
                        color.A = 128;
                        color.R = sb.Color.R;
                        color.G = sb.Color.G;
                        color.B = sb.Color.B;

                        brush = new SolidColorBrush(color);
                    }
                    else
                    {
                        brush = s.Color;
                    }


                    //brush = new SolidColorBrush(color);
                    //    }
                    //    else
                    //        brush = new SolidColorBrush();
                    //}

                    int i = 0;

                    if (s.Points != null)
                    {
                        int tmpCount = s.Points.Count();

                        if (nbPtsMax < tmpCount)
                            nbPtsMax = tmpCount;

                        foreach (var ps in s.Points)
                        {
                            if (GraphType == GraphTypes.Curve)
                            {
                                //ptEnd.X = (i + 1) * BarSize;
                                ptEnd.X = i * BarSize + BarSize / 2;
                                ptEnd.Y = this.ActualHeight - ps.Point;

                                if (i > 0)
                                {
                                    drawingContext.DrawLine(pen, pt, ptEnd);

                                    if (i == 1)
                                    {
                                        if (SizeRectPoint != null && SizeRectPoint.Width != 0 && SizeRectPoint.Height != 0)
                                        {
                                            drawingContext.DrawRectangle(s.Color, pen, new Rect(new Point(pt.X - (SizeRectPoint.Width / 2), pt.Y - (SizeRectPoint.Height / 2)), SizeRectPoint));
                                        }
                                    }

                                    if (SizeRectPoint != null && SizeRectPoint.Width != 0 && SizeRectPoint.Height != 0)
                                    {
                                        drawingContext.DrawRectangle(s.Color, pen, new Rect(new Point(ptEnd.X - (SizeRectPoint.Width / 2), ptEnd.Y - (SizeRectPoint.Height / 2)), SizeRectPoint));
                                    }

                                    if (s.Selected == true)
                                    {
                                        ////////////
                                        // hilite //
                                        ////////////

                                        List<Point> lstPt = new List<Point>();

                                        lstPt.Add(new Point(pt.X, this.ActualHeight));
                                        lstPt.Add(pt);
                                        lstPt.Add(ptEnd);
                                        lstPt.Add(new Point(ptEnd.X, this.ActualHeight));
                                        lstPt.Add(new Point(pt.X, this.ActualHeight));

                                        PathGeometry geometry = new PathGeometry();
                                        PathFigure figure = new PathFigure();
                                        figure.StartPoint = lstPt[0];
                                        lstPt.Remove(lstPt[0]);
                                        figure.Segments.Add(new PolyLineSegment(lstPt, false)); // or true
                                        geometry.Figures.Add(figure);


                                        drawingContext.DrawGeometry(brush, null, geometry);
                                    }                                   
                                }
                                else if (s.Points.Count() == 1)
                                {
                                    if (SizeRectPoint != null && SizeRectPoint.Width != 0 && SizeRectPoint.Height != 0)
                                    {
                                        drawingContext.DrawRectangle(s.Color, pen, new Rect(new Point(ptEnd.X - (SizeRectPoint.Width / 2), ptEnd.Y - (SizeRectPoint.Height / 2)), SizeRectPoint));
                                    }
                                }

                                // hilite
                                if (serieOver == s)
                                {
                                    if (ptOver == i)
                                    {
                                        ////////////
                                        // hilite //
                                        ////////////

                                        //List<Point> lstPt = new List<Point>();

                                        //lstPt.Add(new Point(pt.X, this.ActualHeight));
                                        //lstPt.Add(pt);
                                        //lstPt.Add(ptEnd);
                                        //lstPt.Add(new Point(ptEnd.X, this.ActualHeight));
                                        //lstPt.Add(new Point(pt.X, this.ActualHeight));

                                        //PathGeometry geometry = new PathGeometry();
                                        //PathFigure figure = new PathFigure();
                                        //figure.StartPoint = lstPt[0];
                                        //lstPt.Remove(lstPt[0]);
                                        //figure.Segments.Add(new PolyLineSegment(lstPt, false)); // or true
                                        //geometry.Figures.Add(figure);


                                        //drawingContext.DrawGeometry(brush, null, geometry);

                                        /////////////
                                        // le text //
                                        /////////////

                                        // Affichage des valeurs
                                        FormattedText FText = new FormattedText(ps.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, System.Windows.Media.Brushes.Black);

                                        //Point ptVal = new Point(pt.X - ((FText.Height - BarSize) / 2), ptEnd.Y - 5);
                                        Point ptVal = new Point(ptEnd.X - ((FText.Height) / 2), ptEnd.Y - SizeRectPoint.Height / 2 - 5);

                                        RotateTransform RT = new RotateTransform();
                                        RT.CenterX = ptVal.X;
                                        RT.CenterY = ptVal.Y;
                                        RT.Angle = -90;
                                        drawingContext.PushTransform(RT);

                                        drawingContext.DrawText(FText, ptVal);

                                        drawingContext.Pop();
                                    }
                                }

                                pt.X = ptEnd.X;
                                pt.Y = ptEnd.Y;

                            }
                            else if (GraphType == GraphTypes.CurveStart0)
                            {
                                ptEnd.X = (i + 1) * BarSize;
                                //ptEnd.X = i * BarSize;
                                ptEnd.Y = this.ActualHeight - ps.Point;

                                //if (i > 0)
                                //{
                                drawingContext.DrawLine(pen, pt, ptEnd);

                                //if (i == 0)
                                //{
                                //    if (SizeRectPoint != null && SizeRectPoint.Width != 0 && SizeRectPoint.Height != 0)
                                //    {
                                //        drawingContext.DrawRectangle(s.Color, pen, new Rect(new Point(pt.X - (SizeRectPoint.Width / 2), pt.Y - (SizeRectPoint.Height / 2)), SizeRectPoint));
                                //    }
                                //}

                                if (SizeRectPoint != null && SizeRectPoint.Width != 0 && SizeRectPoint.Height != 0)
                                {
                                    drawingContext.DrawRectangle(s.Color, pen, new Rect(new Point(ptEnd.X - (SizeRectPoint.Width / 2), ptEnd.Y - (SizeRectPoint.Height / 2)), SizeRectPoint));
                                }

                                if (s.Selected == true)
                                {
                                    ////////////
                                    // hilite //
                                    ////////////

                                    List<Point> lstPt = new List<Point>();

                                    lstPt.Add(new Point(pt.X, this.ActualHeight));
                                    lstPt.Add(pt);
                                    lstPt.Add(ptEnd);
                                    lstPt.Add(new Point(ptEnd.X, this.ActualHeight));
                                    lstPt.Add(new Point(pt.X, this.ActualHeight));

                                    PathGeometry geometry = new PathGeometry();
                                    PathFigure figure = new PathFigure();
                                    figure.StartPoint = lstPt[0];
                                    lstPt.Remove(lstPt[0]);
                                    figure.Segments.Add(new PolyLineSegment(lstPt, false)); // or true
                                    geometry.Figures.Add(figure);


                                    drawingContext.DrawGeometry(brush, null, geometry);
                                }

                                // hilite
                                if (serieOver == s)
                                {
                                    if (ptOver == i)
                                    {
                                        ////////////
                                        // hilite //
                                        ////////////

                                        List<Point> lstPt = new List<Point>();

                                        lstPt.Add(new Point(pt.X, this.ActualHeight));
                                        lstPt.Add(pt);
                                        lstPt.Add(ptEnd);
                                        lstPt.Add(new Point(ptEnd.X, this.ActualHeight));
                                        lstPt.Add(new Point(pt.X, this.ActualHeight));

                                        PathGeometry geometry = new PathGeometry();
                                        PathFigure figure = new PathFigure();
                                        figure.StartPoint = lstPt[0];
                                        lstPt.Remove(lstPt[0]);
                                        figure.Segments.Add(new PolyLineSegment(lstPt, false)); // or true
                                        geometry.Figures.Add(figure);


                                        drawingContext.DrawGeometry(brush, null, geometry);

                                        /////////////
                                        // le text //
                                        /////////////

                                        // Affichage des valeurs
                                        FormattedText FText = new FormattedText(ps.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, System.Windows.Media.Brushes.Black);

                                        Point ptVal = new Point(pt.X - ((FText.Height - BarSize) / 2), ptEnd.Y - 5);

                                        RotateTransform RT = new RotateTransform();
                                        RT.CenterX = ptVal.X;
                                        RT.CenterY = ptVal.Y;
                                        RT.Angle = -90;
                                        drawingContext.PushTransform(RT);

                                        drawingContext.DrawText(FText, ptVal);

                                        drawingContext.Pop();
                                    }
                                }
                                //}

                                pt.X = ptEnd.X;
                                pt.Y = ptEnd.Y;

                            }
                            else if (GraphType == GraphTypes.Histogram)
                            {
                                ptEnd.X = (i + 1) * BarSize;
                                ptEnd.Y = this.ActualHeight - ps.Point;

                                if (ps.Point > 0)
                                {
                                    int x = i * BarSize + BeforeSpaceBar;
                                    int y = ((int)this.ActualHeight) - ((int)ps.Point);
                                    int width = BarSize - BeforeSpaceBar - AfterSpaceBar;
                                    int height = ((int)ps.Point) - 1;

                                    if (width < 0)
                                        width = 0;

                                    if (height < 0)
                                        height = 0;

                                    double decal = 0;

                                    if (pen.Thickness % 2 != 0)
                                        decal = 0.5;

                                    Brush brushTmp = null;

                                    if (s.Selected == true)
                                    {
                                        brushTmp = brush;
                                    }

                                    if (serieOver == s)
                                    {
                                        if (ptOver == i)
                                        {
                                            brushTmp = brush;

                                            drawingContext.DrawRectangle(brushTmp, pen, new Rect(x + decal, y + decal, width, height));

                                            /////////////
                                            // le text //
                                            /////////////

                                            // Affichage des valeurs
                                            FormattedText FText = new FormattedText(ps.Label, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, System.Windows.Media.Brushes.Black);

                                            Point ptVal = new Point(x - ((FText.Height - BarSize) / 2), y - 5);

                                            RotateTransform RT = new RotateTransform();
                                            RT.CenterX = ptVal.X;
                                            RT.CenterY = ptVal.Y;
                                            RT.Angle = -90;
                                            drawingContext.PushTransform(RT);

                                            drawingContext.DrawText(FText, ptVal);

                                            drawingContext.Pop();
                                        }
                                        else
                                            drawingContext.DrawRectangle(brushTmp, pen, new Rect(x + decal, y + decal, width, height));
                                    }
                                    else
                                        drawingContext.DrawRectangle(brushTmp, pen, new Rect(x + decal, y + decal, width, height));


                                    pt.X = x;
                                    pt.Y = y;
                                }

                            }

                            //// Pour le text
                            //if (serieOver == s)
                            //{
                            //    if (ptOver == i)
                            //    {
                            //        Point ptVal = new Point(pt.X - (SystemFonts.MessageFontSize / 2), pt.Y);

                            //        RotateTransform RT = new RotateTransform();
                            //        RT.CenterX = ptVal.X;
                            //        RT.CenterY = ptVal.Y;
                            //        RT.Angle = -90;
                            //        drawingContext.PushTransform(RT);

                            //        // Affichage des valeurs
                            //        FormattedText FText = new FormattedText(ps.ToString(), CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(SystemFonts.MessageFontFamily.ToString()), SystemFonts.MessageFontSize, System.Windows.Media.Brushes.Black);
                            //        drawingContext.DrawText(FText, ptVal);


                            //        drawingContext.Pop();

                            //    }
                            //}

                            //if (GraphSize.Width < ptEnd.X)
                            //    GraphSize.Width = ptEnd.X;

                            //if (GraphSize.Height < ps.Point)
                            //    GraphSize.Height = ps.Point;


                            //if (nbPtsMax < i)
                            //    nbPtsMax = i;
                            
                            i++;
                        }
                    }
                }
            }

            if (GraphSize.Width < ptEnd.X)
                GraphSize.Width = nbPtsMax * BarSize;

            // Close the DrawingContext to persist changes to the DrawingVisual.
            drawingContext.Close();

            return drawingVisual;
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (GraphSize.Width != 0 || GraphSize.Height != 0)
                return GraphSize;

            if (Series != null)
            {
                foreach (var s in Series)
                {
                    if (s.Points != null)
                    {
                        int i = 0;
                        foreach (var ps in s.Points)
                        {
                            if (GraphSize.Width < i * BarSize)
                                GraphSize.Width = i * BarSize;

                            if (GraphSize.Height < ps.Point)
                                GraphSize.Height = ps.Point;

                            i++;
                        }
                    }
                }
            }

            return GraphSize;
        }

        // Provide a required override for the VisualChildrenCount property.
        protected override int VisualChildrenCount
        {
            get { return childrenCount; }
        }

        // Provide a required override for the GetVisualChild method.
        protected override Visual GetVisualChild(int index)
        {
            if (index < 0 || index > childrenCount)
            {
                throw new ArgumentOutOfRangeException();
            }

            return _children[index];
        }

        //
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        public void ResetEvent()
        {
            PropertyChanged = null;
        }

        public class Serie
        {
            public string Name;
            public Brush Color;
            public bool Selected = false;
            public IEnumerable<Value> Points;

            public class Value
            {
                public double Point;
                public string Label;
            }
        }
    }
}
