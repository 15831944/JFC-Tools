using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Diagnostics;

namespace JFCGridControl
{
    public class JFCVisualBrush : FrameworkElement
    {

        public Visual Visual
        {
            get { return (Visual)GetValue(VisualProperty); }
            set { SetValue(VisualProperty, value);}
        }        

        // Using a DependencyProperty as the backing store for Visual.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty VisualProperty =
            DependencyProperty.Register("Visual", typeof(Visual), typeof(JFCVisualBrush), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateVisual)));


        public int UpdateMilliseconde
        {
            get { return (int)GetValue(UpdateMillisecondeProperty); }
            set { SetValue(UpdateMillisecondeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UpdateMilliseconde.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UpdateMillisecondeProperty =
            DependencyProperty.Register("UpdateMilliseconde", typeof(int), typeof(JFCVisualBrush), new UIPropertyMetadata(1, new PropertyChangedCallback(UpdateVisual)));



        public bool UpdateFrame
        {
            get { return (bool)GetValue(UpdateFrameProperty); }
            set { SetValue(UpdateFrameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UpdateFrame.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UpdateFrameProperty =
            DependencyProperty.Register("UpdateFrame", typeof(bool), typeof(JFCVisualBrush), new UIPropertyMetadata(true));



        private VisualCollection _children;
        DispatcherTimer timer;
        //PerformanceCounter _perfCpu;

        public JFCVisualBrush()
        {
            _children = new VisualCollection(this);
            timer = new DispatcherTimer();
            this.SizeChanged += new SizeChangedEventHandler(JFCVisualBrush_SizeChanged);

            //_perfCpu = new PerformanceCounter("Processor", "% Processor Time"t, "_Total", true);
        }

        void JFCVisualBrush_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            JFCVisualBrush v = sender as JFCVisualBrush;
            v._children.Clear();
            v._children.Add(v.CreateDrawingVisual());
        }

        private static void UpdateVisual(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCVisualBrush v = obj as JFCVisualBrush;

            v._children.Clear();

            if (v.Visual != null)
            {

               // if (v.Visual is FrameworkElement)
               //{
               //    ((FrameworkElement)v.Visual).LayoutUpdated += delegate(object sender, EventArgs e1)
               //     {
               //         if (sender != null)
               //         {
               //             int i = 0;
               //         }
               //     };
               //}

                // on lance une première fois le dessin
                v._children.Add(v.CreateDrawingVisual());

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

        }

        static void timer_Tick(object sender, EventArgs e)
        {

            DispatcherTimer t = sender as DispatcherTimer;
            JFCVisualBrush v = t.Tag as JFCVisualBrush;

            if (v.UpdateFrame == true)
            {
                v._children.Clear();
                v._children.Add(v.CreateDrawingVisual());
            }
        }

        private DrawingVisual CreateDrawingVisual()
        {

            //Capture the image of the visual in the same size as Printing page.

            //RenderTargetBitmap bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Pbgra32);


            if (this.Visual is FrameworkElement)
            {

                FrameworkElement element = this.Visual as FrameworkElement;

                RenderTargetBitmap bmp = null;
                Size sz = new Size(0.0, 0.0);

                if (element.ActualWidth > 0 && element.ActualHeight > 0)
                {
                    bmp = new RenderTargetBitmap((int)element.ActualWidth, (int)element.ActualHeight, 96, 96, PixelFormats.Pbgra32);

                    double RapportH = element.ActualWidth / this.ActualWidth;
                    double RapportV = element.ActualHeight / this.ActualHeight;

                    if (RapportH >= RapportV)
                    {
                        sz = new Size((int)this.ActualWidth, (int)Math.Round((element.ActualHeight / RapportH), MidpointRounding.ToEven));
                    }
                    else
                    {
                        sz = new Size((int)Math.Round((element.ActualWidth / RapportV), MidpointRounding.ToEven), (int)this.ActualHeight);
                    }
                }
                else
                {
                    if (this.ActualWidth > 0.0 && this.ActualHeight > 0.0)
                    {
                        bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Pbgra32);

                        sz = new Size((int)this.ActualWidth, this.ActualHeight);

                        element.Measure(new Size(1024, 768));
                        element.UpdateLayout();
                    }
                }

                if (bmp != null)
                {
                    //element.BitmapEffect

                    bmp.Render(element);

                    //create new visual which would be initialized by image

                    DrawingVisual drawingVisual = new DrawingVisual();

                    DrawingContext dc = drawingVisual.RenderOpen();

                    dc.DrawImage(bmp, new Rect(sz));

                    dc.Close();

                    bmp = null;

                    GC.Collect();

                    return drawingVisual;
                }
            }

            return null;
        }



        // Provide a required override for the VisualChildrenCount property.
        protected override int VisualChildrenCount
        {
            get { return _children.Count; }
        }

        // Provide a required override for the GetVisualChild method.
        protected override Visual GetVisualChild(int index)
        {
            if (index < 0 || index > _children.Count)
            {
                throw new ArgumentOutOfRangeException();
            }

            return _children[index];
        }
    }
}
