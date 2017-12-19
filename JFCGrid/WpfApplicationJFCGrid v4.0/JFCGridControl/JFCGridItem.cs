// JFCGridControl.JFCGridItem
using JFCGridControl;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;


namespace JFCGridControl
{
    public class JFCGridItem : ContentControl
    {
        internal bool CallIsMouseOverLineCallBack = true;

        public static readonly DependencyProperty IsMouseOverLineProperty = DependencyProperty.Register("IsMouseOverLine", typeof(bool), typeof(JFCGridItem), new UIPropertyMetadata(false, JFCGridItem.UpdateIsMouseOverLine));

        internal bool CallIsMouseOverColumnCallBack = true;

        public static readonly DependencyProperty IsMouseOverColumnProperty = DependencyProperty.Register("IsMouseOverColumn", typeof(bool), typeof(JFCGridItem), new UIPropertyMetadata(false, JFCGridItem.UpdateIsMouseOverColumn));

        public static readonly DependencyProperty IsSelectedProperty = DependencyProperty.Register("IsSelected", typeof(bool), typeof(JFCGridItem), new UIPropertyMetadata(false));

        public static readonly DependencyProperty IsAlternateProperty = DependencyProperty.Register("IsAlternate", typeof(bool), typeof(JFCGridItem), new UIPropertyMetadata(false));

        public static readonly DependencyProperty OrientationProperty = DependencyProperty.Register("Orientation", typeof(Orientation), typeof(JFCGridItem), new UIPropertyMetadata(Orientation.Horizontal, JFCGridItem.UpdateOrientation));

        private JFCExpendItem data;

        public static readonly DependencyProperty SourceProperty = DependencyProperty.Register("Source", typeof(object), typeof(JFCGridItem), new UIPropertyMetadata("test"));

        public JFCGrid ParentGrid
        {
            get;
            set;
        }

        public List<JFCGridItem> Peer
        {
            get;
            set;
        }

        public int IndexData
        {
            get;
            set;
        }

        public bool IsMouseOverLine
        {
            get
            {
                return (bool)base.GetValue(JFCGridItem.IsMouseOverLineProperty);
            }
            set
            {
                base.SetValue(JFCGridItem.IsMouseOverLineProperty, value);
            }
        }

        public bool IsMouseOverColumn
        {
            get
            {
                return (bool)base.GetValue(JFCGridItem.IsMouseOverColumnProperty);
            }
            set
            {
                base.SetValue(JFCGridItem.IsMouseOverColumnProperty, value);
            }
        }

        public bool IsSelected
        {
            get
            {
                return (bool)base.GetValue(JFCGridItem.IsSelectedProperty);
            }
            set
            {
                base.SetValue(JFCGridItem.IsSelectedProperty, value);
            }
        }

        public bool IsAlternate
        {
            get
            {
                return (bool)base.GetValue(JFCGridItem.IsAlternateProperty);
            }
            set
            {
                base.SetValue(JFCGridItem.IsAlternateProperty, value);
            }
        }

        public Orientation Orientation
        {
            get
            {
                return (Orientation)base.GetValue(JFCGridItem.OrientationProperty);
            }
            set
            {
                base.SetValue(JFCGridItem.OrientationProperty, value);
            }
        }

        internal JFCExpendItem Data
        {
            get
            {
                return this.data;
            }
            set
            {
                this.data = value;
                if (this.data != null)
                {
                    this.Source = this.data.Obj;
                }
                else
                {
                    this.Source = null;
                }
            }
        }

        public object Source
        {
            get
            {
                return base.GetValue(JFCGridItem.SourceProperty);
            }
            private set
            {
                base.SetValue(JFCGridItem.SourceProperty, value);
            }
        }

        public event RoutedEventHandler IsMouseOverLineChanged;

        public event RoutedEventHandler IsMouseOverColumnChanged;

        public JFCGridItem(JFCGrid parent)
        {
            base.ClipToBounds = true;
            base.SnapsToDevicePixels = true;
            base.Focusable = false;
            this.Peer = new List<JFCGridItem>();
            this.ParentGrid = parent;
        }

        public void SelectedPeer(bool isSelected)
        {
            foreach (JFCGridItem item in this.Peer)
            {
                item.IsSelected = isSelected;
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseEnter(e);
        }

        public void MouseEnterItem()
        {
            if (this.Orientation == Orientation.Horizontal)
            {
                if (!(bool)base.GetValue(JFCGridItem.IsMouseOverLineProperty))
                {
                    base.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                    if (this.Peer != null)
                    {
                        foreach (JFCGridItem item in this.Peer)
                        {
                            item.CallIsMouseOverLineCallBack = false;
                            item.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                            item.CallIsMouseOverLineCallBack = true;
                        }
                    }
                }
            }
            else
            {
                Orientation orientation = this.Orientation;
            }
        }

        public void MouseLeaveItem()
        {
            if (this.Orientation == Orientation.Horizontal)
            {
                if ((bool)base.GetValue(JFCGridItem.IsMouseOverLineProperty))
                {
                    base.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                    if (this.Peer != null)
                    {
                        foreach (JFCGridItem item in this.Peer)
                        {
                            item.CallIsMouseOverLineCallBack = false;
                            item.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                            item.CallIsMouseOverLineCallBack = true;
                        }
                    }
                }
            }
            else
            {
                Orientation orientation = this.Orientation;
            }
        }

        protected override void OnMouseEnter(MouseEventArgs e)
        {
            base.OnMouseEnter(e);
            if (this.Orientation == Orientation.Horizontal)
            {
                base.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                if (this.Peer != null)
                {
                    foreach (JFCGridItem item in this.Peer)
                    {
                        item.CallIsMouseOverLineCallBack = false;
                        item.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                        item.CallIsMouseOverLineCallBack = true;
                    }
                }
            }
            else
            {
                Orientation orientation = this.Orientation;
            }
        }

        protected override void OnMouseLeave(MouseEventArgs e)
        {
            base.OnMouseLeave(e);
            if (this.Orientation == Orientation.Horizontal)
            {
                base.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                if (this.Peer != null)
                {
                    foreach (JFCGridItem item in this.Peer)
                    {
                        item.CallIsMouseOverLineCallBack = false;
                        item.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                        item.CallIsMouseOverLineCallBack = true;
                    }
                }
            }
            else
            {
                Orientation orientation = this.Orientation;
            }
        }

        private static void UpdateOrientation(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem jFCGridItem = obj as JFCGridItem;
            if (jFCGridItem.Orientation == Orientation.Vertical)
            {
                jFCGridItem.MouseMove += JFCGridItem.Item_MouseMove;
            }
            else
            {
                jFCGridItem.MouseMove -= JFCGridItem.Item_MouseMove;
            }
        }

        private static void Item_MouseMove(object sender, MouseEventArgs e)
        {
            JFCGridItem jFCGridItem = sender as JFCGridItem;
            if (jFCGridItem.ParentGrid != null)
            {
                Point position = e.GetPosition(jFCGridItem);
                double num = position.Y % (double)jFCGridItem.ParentGrid.Manager.Parent.RowHeight;
                double num2 = (position.Y - num) / (double)jFCGridItem.ParentGrid.Manager.Parent.RowHeight;
                if (num > 0.0)
                {
                    num2 += 1.0;
                }
                int num3 = 0;
                foreach (LinkedList<UIElement> item in jFCGridItem.ParentGrid.Manager.TabLineB)
                {
                    IEnumerable<JFCGridItem> source = from JFCGridItem ctrl in
                                                          from ctrl in item
                                                          where ctrl is JFCGridItem
                                                          select ctrl
                                                      where ctrl.Orientation == Orientation.Horizontal
                                                      select ctrl;
                    if (source.Count() > 0)
                    {
                        if (num3 == (int)num2 - 1)
                        {
                            source.First().MouseEnterItem();
                        }
                        else
                        {
                            source.First().MouseLeaveItem();
                        }
                    }
                    num3++;
                }
            }
        }

        internal void OnIsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            if (this.IsMouseOverLineChanged != null)
            {
                this.IsMouseOverLineChanged(this, e);
            }
        }

        private static void UpdateIsMouseOverLine(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem jFCGridItem = obj as JFCGridItem;
            if (jFCGridItem.CallIsMouseOverLineCallBack)
            {
                jFCGridItem.OnIsMouseOverLineChanged(jFCGridItem, new RoutedEventArgs());
            }
        }

        internal void OnIsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
            if (this.IsMouseOverColumnChanged != null)
            {
                this.IsMouseOverColumnChanged(this, e);
            }
        }

        private static void UpdateIsMouseOverColumn(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem jFCGridItem = obj as JFCGridItem;
            if (jFCGridItem.CallIsMouseOverColumnCallBack)
            {
                jFCGridItem.OnIsMouseOverColumnChanged(jFCGridItem, new RoutedEventArgs());
            }
        }
    }
}