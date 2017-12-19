using System;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using System.Windows.Threading;
using System.Diagnostics;
using System.Windows.Shapes;


namespace JFCGridControl
{
    
    public class JFCVisualBrush2 : Panel
    {

        public Visual Visual
        {
            get { return (Visual)GetValue(VisualProperty); }
            set { SetValue(VisualProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Visual.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty VisualProperty =
            DependencyProperty.Register("Visual", typeof(Visual), typeof(JFCVisualBrush2), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateVisual)));


        public int UpdateMilliseconde
        {
            get { return (int)GetValue(UpdateMillisecondeProperty); }
            set { SetValue(UpdateMillisecondeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UpdateMilliseconde.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UpdateMillisecondeProperty =
            DependencyProperty.Register("UpdateMilliseconde", typeof(int), typeof(JFCVisualBrush2), new UIPropertyMetadata(1, new PropertyChangedCallback(UpdateVisual)));
        

        //private VisualCollection _children;
        DispatcherTimer timer;
        //PerformanceCounter _perfCpu;

        VisualBrush _brush;

        public JFCVisualBrush2()
        {
            //_children = new VisualCollection(this);
            timer = new DispatcherTimer();

            //this.Background = Brushes.Red;

            _brush = new VisualBrush();

            this.Background = _brush;       

            //_perfCpu = new PerformanceCounter("Processor", "% Processor Time"t, "_Total", true);
        }

        private static void UpdateVisual(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCVisualBrush2 v = obj as JFCVisualBrush2;

            //v._children.Clear();

            if (v.Visual != null)
            {

                v._brush.Visual = v.Visual;                

                v.timer.Interval = new TimeSpan(0, 0, 0, 0, v.UpdateMilliseconde);

                v.timer.Tick -= timer_Tick;

                v.timer.Tag = v;

                v.timer.Tick += new EventHandler(timer_Tick);

                //v.timer.Tick += (senderTick, argTick) =>
                //{
                //    //float t = v._perfCpu.NextValue();

                //    //if (t < 50)
                //    //{

                //    //}                   

                //};

                v.timer.IsEnabled = true;


            }
            else
            {
                v._brush.Visual = null;
            }

           
        }

        static void timer_Tick(object sender, EventArgs e)
        {
            DispatcherTimer t = sender as DispatcherTimer;
            JFCVisualBrush2 v = t.Tag as JFCVisualBrush2;

            //v._brush.AutoLayoutContent = true;

            //v.Background = v._brush;

            // = new VisualBrush(v.Visual);

            //v.Background = br;

            //TileBrush tl = br.Clone();
            
            //Freezable f  = br.GetAsFrozen();

            //Brush b = f as Brush;

            //DrawingImage di = new DrawingImage();
            //DrawingVisual dv = new DrawingVisual();
            //dv.TransformToVisual(v.Visual);



            //v.Source = br;

            //Brush b = br as Brush;
            //TileBrush ti = br;

            //v.Background = b;
            
            //v._children.Clear();
            //v._children.Add(v.CreateDrawingVisual());
        }

        //private DrawingVisual CreateDrawingVisual()
        //{

        //    //Capture the image of the visual in the same size as Printing page.

        //    //RenderTargetBitmap bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Pbgra32);


        //    if (this.Visual is FrameworkElement)
        //    {

        //        FrameworkElement element = this.Visual as FrameworkElement;

        //        RenderTargetBitmap bmp = new RenderTargetBitmap((int)element.ActualWidth, (int)element.ActualHeight, 96, 96, PixelFormats.Pbgra32);

        //        //element.BitmapEffect

        //        bmp.Render(element);

        //        //create new visual which would be initialized by image

        //        DrawingVisual drawingVisual = new DrawingVisual();

        //        Size sz;

        //        double RapportH = element.ActualWidth / this.ActualWidth;
        //        double RapportV = element.ActualHeight / this.ActualHeight;

        //        if (RapportH >= RapportV)
        //        {
        //            sz = new Size((int)this.ActualWidth, (int)Math.Round((element.ActualHeight / RapportH), MidpointRounding.ToEven));
        //        }
        //        else
        //        {
        //            sz = new Size((int)Math.Round((element.ActualWidth / RapportV), MidpointRounding.ToEven), (int)this.ActualHeight);
        //        }

        //        DrawingContext dc = drawingVisual.RenderOpen();

        //        dc.DrawImage(bmp, new Rect(sz));

        //        dc.Close();


        //        return drawingVisual;
        //    }

        //    return null;
        //}

        //// Provide a required override for the VisualChildrenCount property.
        //protected override int VisualChildrenCount
        //{
        //    get { return _children.Count; }
        //}

        //// Provide a required override for the GetVisualChild method.
        //protected override Visual GetVisualChild(int index)
        //{
        //    if (index < 0 || index > _children.Count)
        //    {
        //        throw new ArgumentOutOfRangeException();
        //    }

        //    return _children[index];
        //}        
    }
}
