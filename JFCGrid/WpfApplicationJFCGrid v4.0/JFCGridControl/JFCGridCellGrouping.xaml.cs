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

namespace JFCGridControl
{
    /// <summary>
    /// Interaction logic for JFCGridCellGrouping.xaml
    /// </summary>
    public partial class JFCGridCellGrouping : UserControl
    {
        public FrameworkElement Component
        {
            get { return (FrameworkElement)GetValue(ComponentProperty); }
            set { SetValue(ComponentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Component.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ComponentProperty =
            DependencyProperty.Register("Component", typeof(FrameworkElement), typeof(JFCGridCellGrouping), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateComponent)));

        public Boolean IsSelected
        {
            get { return (Boolean)GetValue(IsSelectedProperty); }
            set { SetValue(IsSelectedProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsSelected.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsSelectedProperty =
            DependencyProperty.Register("IsSelected", typeof(Boolean), typeof(JFCGridCellGrouping), new UIPropertyMetadata(false));

        public JFCGridCellGrouping()
        {
            InitializeComponent();
        }

        private static void UpdateComponent(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCellGrouping cg = obj as JFCGridCellGrouping;
            
            cg.MyComponent.Children.Clear();

            if (cg.Component != null)
            {
                cg.MyComponent.Children.Add(cg.Component);

                cg.Component.Tag = cg;
            }            
        }
    }
}
