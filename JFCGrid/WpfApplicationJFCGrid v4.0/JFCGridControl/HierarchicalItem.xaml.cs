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
using System.Globalization;

namespace JFCGridControl
{
    /// <summary>
    /// Interaction logic for HierarchicalItem.xaml
    /// </summary>
    public partial class HierarchicalItem : UserControl
    {


        public FrameworkElement Component
        {
            get { return (FrameworkElement)GetValue(ComponentProperty); }
            set { SetValue(ComponentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Component.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ComponentProperty =
            DependencyProperty.Register("Component", typeof(FrameworkElement), typeof(HierarchicalItem), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateComponent)));


        public HierarchicalItem()
        {
            InitializeComponent();
        }

        private static void UpdateComponent(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            HierarchicalItem hi = obj as HierarchicalItem;

            //hi.MyComponent.Content = hi.Component;

            hi.MyComponent.Children.Clear();

            if (hi.Component != null)
            {
                //hi.Component.ContextMenu = hi.ContextMenu;
                hi.MyComponent.Children.Add(hi.Component);
            }
        }

        //private void MyButton_Click(object sender, RoutedEventArgs e)
        //{
        //    Button btn = sender as Button;

        //    JFCExpendItem item = btn.DataContext as JFCExpendItem;

        //    if (item.IsExpended)
        //        item.IsExpended = false;
        //    else
        //        item.IsExpended = true;
        //}

        private void MyButton_MouseDown(object sender, MouseButtonEventArgs e)
        {            
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                //ContentControl btn = sender as ContentControl;
                Control btn = sender as Control;

                JFCExpendItem item = btn.DataContext as JFCExpendItem;

                if (item.IsExpended)
                    item.IsExpended = false;
                else
                    item.IsExpended = true;
            }
        }
    }

    [ValueConversion(typeof(Int16), typeof(Thickness))]
    public class MarginConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            Int16 level = (Int16)value;
            return new Thickness(level * 16, 0, 0, 0);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            new NotImplementedException();
            return null;
        }
    }

}
