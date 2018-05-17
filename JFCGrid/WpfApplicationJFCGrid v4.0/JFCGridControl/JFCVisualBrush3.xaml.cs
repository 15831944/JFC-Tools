using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Drawing;

namespace JFCGridControl
{
    /// <summary>
    /// Interaction logic for VisualBrush3.xaml
    /// </summary>
    public partial class JFCVisualBrush3 : UserControl
    {
        public Visual Visual
        {
            get { return (Visual)GetValue(VisualProperty); }
            set { SetValue(VisualProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Visual.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty VisualProperty =
            DependencyProperty.Register("Visual", typeof(Visual), typeof(JFCVisualBrush3), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateVisual)));


        public int UpdateMilliseconde
        {
            get { return (int)GetValue(UpdateMillisecondeProperty); }
            set { SetValue(UpdateMillisecondeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UpdateMilliseconde.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UpdateMillisecondeProperty =
            DependencyProperty.Register("UpdateMilliseconde", typeof(int), typeof(JFCVisualBrush3), new UIPropertyMetadata(1, new PropertyChangedCallback(UpdateVisual)));

        public bool UpdateFrame
        {
            get { return (bool)GetValue(UpdateFrameProperty); }
            set { SetValue(UpdateFrameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UpdateFrame.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UpdateFrameProperty =
            DependencyProperty.Register("UpdateFrame", typeof(bool), typeof(JFCVisualBrush3), new UIPropertyMetadata(true));

        public Uri UriIcon
        {
            get { return (Uri)GetValue(UriIconProperty); }
            set { SetValue(UriIconProperty, value); }
        }

        // Using a DependencyProperty as the backing store for UriIcon.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty UriIconProperty =
            DependencyProperty.Register("UriIcon", typeof(Uri), typeof(JFCVisualBrush3), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateIcon)));



        private ImageSource SourceImage
        {
            get { return (ImageSource)GetValue(SourceImageProperty); }
            set { SetValue(SourceImageProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Source.  This enables animation, styling, binding, etc...
        private static readonly DependencyProperty SourceImageProperty =
            DependencyProperty.Register("SourceImage", typeof(ImageSource), typeof(JFCVisualBrush3), new UIPropertyMetadata(null));


        DispatcherTimer timer;
        //PerformanceCounter _perfCpu;
        

        public JFCVisualBrush3()
        {
            InitializeComponent();

            MyImage.DataContext = this;

            MyImage.Loaded += new RoutedEventHandler(MyImage_Loaded);
        }

        void MyImage_Loaded(object sender, RoutedEventArgs e)
        {

            ImageSource img = UpdateImage();

            if (img != null)
            {
                //if (img.Width < this.ActualWidth || img.Height < this.ActualHeight)
                //    MyImage.Stretch = Stretch.None;
                //else
                //    MyImage.Stretch = Stretch.Uniform;

                //SourceImage = UpdateImage();

                SourceImage = img;
            }
        }

        private static void UpdateIcon(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCVisualBrush3 v = obj as JFCVisualBrush3; 
            //v.SourceImage = v.UpdateImage();

            ImageSource img = v.UpdateImage();

            if (img != null)
            {
                //if (img.Width < v.ActualWidth || img.Height < v.ActualHeight)
                //    v.MyImage.Stretch = Stretch.None;
                //else
                //    v.MyImage.Stretch = Stretch.Uniform;

                v.SourceImage = img;
            }

           
        }

        private static void UpdateVisual(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCVisualBrush3 v = obj as JFCVisualBrush3;

            if (v.Visual != null)
            {

                // on lance une première fois le dessin
                v.SourceImage = v.UpdateImage();

                if (v.timer == null)
                    v.timer = new DispatcherTimer();

                v.timer.Interval = new TimeSpan(0, 0, 0, 0, v.UpdateMilliseconde);

                //v.timer.Tick -= timer_Tick;

                v.timer.Tag = v;

                v.timer.Tick += new EventHandler(timer_Tick);

                v.timer.IsEnabled = true;
                v.timer.Start();
            }
            else
            {
                if (v.timer != null)
                {
                    v.timer.Stop();
                    v.timer.Tick -= timer_Tick;
                    v.timer.Tag = null;
                    v.timer = null;
                }
            }

        }

        static void timer_Tick(object sender, EventArgs e)
        {

            DispatcherTimer t = sender as DispatcherTimer;
            JFCVisualBrush3 v = t.Tag as JFCVisualBrush3;


            if (v.UpdateFrame == true)
            {
                //v.SourceImage = v.UpdateImage();

                Action handler = null;
                handler = delegate()
                    {
                        //if (v.SourceImage != null)
                        //{
                        //    v.SourceImage.Dispatcher.BeginInvoke(
                        //        (Action)delegate()
                        //        {
                        ImageSource img = v.UpdateImage();

                        //if (img.Width < v.ActualWidth || img.Height < v.ActualHeight)
                        //    v.MyImage.Stretch = Stretch.None;
                        //else
                        //    v.MyImage.Stretch = Stretch.Uniform;

                        v.SourceImage = img;
                        //        });
                        //}
                    };

                v.Dispatcher.BeginInvoke(handler, DispatcherPriority.Background);
            }
        }

        private ImageSource UpdateImage()
        {

            MyRectangle.Visibility = Visibility.Visible;

            if (this.Visual != null)
            {
                if (this.Visual is FrameworkElement)
                {
                    RenderTargetBitmap bmp = null;
                    FrameworkElement element = this.Visual as FrameworkElement;

                    MyRectangle.Visibility = Visibility.Hidden;

                    //Size sz = new Size(0.0, 0.0);

                    if (element.ActualWidth > 0 && element.ActualHeight > 0)
                    {
                        bmp = new RenderTargetBitmap((int)element.ActualWidth, (int)element.ActualHeight, 96, 96, PixelFormats.Pbgra32);
                        //bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Pbgra32);

                        MyImage.Stretch = Stretch.Uniform;
                    }
                    //else if (this.ActualWidth > 0.0 && this.ActualHeight > 0.0)
                    //{

                    //    bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Pbgra32);                         
                    //}
                    else
                    {
                        if (UriIcon == null)
                        {
                            MyRectangle.Visibility = Visibility.Visible;
                        }
                        else
                        {
                            BitmapImage bi = new BitmapImage(UriIcon);
                            return bi;
                        }

                        MyImage.Stretch = Stretch.None;
                    }

                    if (bmp != null)
                    {
                        bmp.Render(element);

                        return bmp;
                    }

                }
            }

            if (UriIcon == null)
            {
                MyRectangle.Visibility = Visibility.Visible;
            }
            else
            {
                BitmapImage bi = new BitmapImage(UriIcon);

                MyImage.Stretch = Stretch.None;

                return bi;
            }

            return null;
        }
    }
}
