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
        public static readonly DependencyProperty ComponentProperty = DependencyProperty.Register("Component", typeof(FrameworkElement), typeof(JFCGridCellGrouping), new UIPropertyMetadata(null, JFCGridCellGrouping.UpdateComponent));

        public static readonly DependencyProperty IsSelectedProperty = DependencyProperty.Register("IsSelected", typeof(bool), typeof(JFCGridCellGrouping), new UIPropertyMetadata(false));

        public FrameworkElement Component
        {
            get
            {
                return (FrameworkElement)base.GetValue(JFCGridCellGrouping.ComponentProperty);
            }
            set
            {
                base.SetValue(JFCGridCellGrouping.ComponentProperty, value);
            }
        }

        public bool IsSelected
        {
            get
            {
                return (bool)base.GetValue(JFCGridCellGrouping.IsSelectedProperty);
            }
            set
            {
                base.SetValue(JFCGridCellGrouping.IsSelectedProperty, value);
            }
        }

        public JFCGridCellGrouping()
        {
            this.InitializeComponent();
        }

        private static void UpdateComponent(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCellGrouping jFCGridCellGrouping = obj as JFCGridCellGrouping;
            jFCGridCellGrouping.MyComponent.Children.Clear();
            if (jFCGridCellGrouping.Component != null)
            {
                jFCGridCellGrouping.MyComponent.Children.Add(jFCGridCellGrouping.Component);
                jFCGridCellGrouping.Component.Tag = jFCGridCellGrouping;
            }
        }
    }
}
