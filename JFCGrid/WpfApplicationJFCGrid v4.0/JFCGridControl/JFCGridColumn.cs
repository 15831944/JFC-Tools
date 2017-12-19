// JFCGridControl.JFCGridColumn
using JFCGridControl;
using System;
using System.Collections.Specialized;
using System.Linq;
using System.Windows;
using System.Windows.Controls;


namespace JFCGridControl
{
    public class JFCGridColumn : JFCGridColumnBase
    {
        public enum FrozenType
        {
            None,
            Start,
            End
        }

        public enum TypesColumn
        {
            Normal,
            Hierarchical,
            GroupingNoRow,
            GroupingWithRow,
            GroupingWithRowWhenNoChildren
        }

        private FrozenType frozen;

        private JFCGridColumnHeader header;

        private bool isResizable = true;

        private bool isMovable = true;

        private bool isSelected;

        private ObservableCollection<JFCGridColumn> childrenColumns = new ObservableCollection<JFCGridColumn>();

        public static readonly DependencyProperty HaveChildrenProperty = DependencyProperty.Register("HaveChildren", typeof(bool), typeof(JFCGridColumn), new UIPropertyMetadata(false));

        public GridLength actualWidthHeader;

        private TypesColumn typeColumn;

        private int levelGrouping = -1;

        private JFCGridItem item;

        private object tag;

        public FrozenType Frozen
        {
            get
            {
                return this.frozen;
            }
            set
            {
                FrozenType frozenType = this.frozen;
                this.frozen = value;
                base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Frozen", frozenType, value));
            }
        }

        public JFCGridColumnHeader Header
        {
            get
            {
                return this.header;
            }
            set
            {
                JFCGridColumnHeader oldValue = this.header;
                this.header = value;
                this.header.Column = this;
                this.header.Click += this.header_Click;
                base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Header", oldValue, value));
            }
        }

        public bool IsResizable
        {
            get
            {
                return this.isResizable;
            }
            set
            {
                bool flag = false;
                if (this.isResizable != value)
                {
                    flag = true;
                }
                bool flag2 = this.isResizable;
                this.isResizable = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("IsResizable", flag2, value));
                }
            }
        }

        public bool IsMovable
        {
            get
            {
                return this.isMovable;
            }
            set
            {
                bool flag = false;
                if (this.isMovable != value)
                {
                    flag = true;
                }
                bool flag2 = this.isMovable;
                this.isMovable = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("IsMovable", flag2, value));
                }
            }
        }

        public bool IsSelected
        {
            get
            {
                return this.isSelected;
            }
            set
            {
                bool flag = false;
                if (this.isSelected != value)
                {
                    flag = true;
                }
                bool flag2 = this.isSelected;
                this.isSelected = value;
                this.item.IsSelected = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("IsSelected", flag2, value));
                    if (this.ChildrenColumns != null)
                    {
                        foreach (JFCGridColumn childrenColumn in this.ChildrenColumns)
                        {
                            childrenColumn.IsSelected = this.isSelected;
                        }
                    }
                }
            }
        }

        public ObservableCollection<JFCGridColumn> ChildrenColumns
        {
            get
            {
                return this.childrenColumns;
            }
        }

        public bool HaveChildren
        {
            get
            {
                return (bool)base.GetValue(JFCGridColumn.HaveChildrenProperty);
            }
            set
            {
                base.SetValue(JFCGridColumn.HaveChildrenProperty, value);
            }
        }

        public GridLength ActualWidthHeader
        {
            get
            {
                return this.actualWidthHeader;
            }
            set
            {
                bool flag = false;
                if (this.actualWidthHeader != value)
                {
                    flag = true;
                }
                GridLength gridLength = this.actualWidthHeader;
                this.actualWidthHeader = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("ActualWidthHeader", gridLength, value));
                }
                if (this.childrenColumns.Count() == 0 && base.actualWidth.Value < this.actualWidthHeader.Value)
                {
                    this.ActualWidth = this.ActualWidthHeader;
                }
            }
        }

        public override GridLength ActualWidth
        {
            get
            {
                if (this.childrenColumns.Count() > 0)
                {
                    double num = 0.0;
                    foreach (JFCGridColumn childrenColumn in this.childrenColumns)
                    {
                        num += childrenColumn.ActualWidth.Value;
                    }
                    return new GridLength(num);
                }
                return base.actualWidth;
            }
            set
            {
                bool flag = false;
                if (base.actualWidth != value)
                {
                    flag = true;
                }
                GridLength actualWidth = base.actualWidth;
                base.actualWidth = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("ActualWidth", actualWidth, value));
                }
            }
        }

        public override GridLength Width
        {
            get
            {
                if (this.childrenColumns.Count() > 0)
                {
                    double num = 0.0;
                    foreach (JFCGridColumn childrenColumn in this.childrenColumns)
                    {
                        num += childrenColumn.Width.Value;
                    }
                    return new GridLength(num);
                }
                return base.width;
            }
            set
            {
                bool flag = false;
                if (base.width != value)
                {
                    flag = true;
                }
                GridLength width = base.width;
                base.width = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Width", width, value));
                }
                if (!base.width.IsAuto)
                {
                    this.ActualWidth = base.width;
                }
            }
        }

        public TypesColumn TypeColumn
        {
            get
            {
                return this.typeColumn;
            }
            set
            {
                bool flag = false;
                if (this.typeColumn != value)
                {
                    flag = true;
                }
                TypesColumn typesColumn = this.typeColumn;
                this.typeColumn = value;
                if (flag)
                {
                    base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("TypeColumn", typesColumn, value));
                }
            }
        }

        public int LevelGrouping
        {
            get
            {
                return this.levelGrouping;
            }
            set
            {
                int num = this.levelGrouping;
                this.levelGrouping = value;
                base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("LevelGrouping", num, value));
            }
        }

        public JFCGridItem Item
        {
            get
            {
                return this.item;
            }
            set
            {
                JFCGridItem oldValue = this.item;
                this.item = value;
                base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Item", oldValue, value));
            }
        }

        public object Tag
        {
            get
            {
                return this.tag;
            }
            set
            {
                object oldValue = this.tag;
                this.tag = value;
                base.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Tag", oldValue, value));
            }
        }

        public event RoutedEventHandler HeaderClick;

        public JFCGridColumn()
        {
            this.Header = new JFCGridColumnHeader();
            this.item = new JFCGridItem(null);
            this.item.Orientation = Orientation.Vertical;
            this.childrenColumns.Parent = this;
            this.childrenColumns.CollectionChanged += this.childrenColumns_CollectionChanged;
        }

        public JFCGridColumn(string BindingPath)
        {
            base.BindingPath = BindingPath;
            this.Header = new JFCGridColumnHeader();
            this.item = new JFCGridItem(null);
            this.item.Orientation = Orientation.Vertical;
            this.childrenColumns.Parent = this;
            this.childrenColumns.CollectionChanged += this.childrenColumns_CollectionChanged;
        }

        public JFCGridColumn(Func<object, object> getterHandler)
            : base(getterHandler)
        {
            this.Header = new JFCGridColumnHeader();
            this.item = new JFCGridItem(null);
            this.item.Orientation = Orientation.Vertical;
            this.childrenColumns.Parent = this;
            this.childrenColumns.CollectionChanged += this.childrenColumns_CollectionChanged;
        }

        private void header_Click(object sender, RoutedEventArgs e)
        {
            this.OnHeaderClick(this, e);
        }

        private void header_SizeChanged(object sender, SizeChangedEventArgs e)
        {
        }

        internal void OnHeaderClick(object sender, RoutedEventArgs e)
        {
            if (this.HeaderClick != null)
            {
                this.HeaderClick(this, e);
            }
        }

        private void childrenColumns_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (this.childrenColumns.Count() > 0)
            {
                this.HaveChildren = true;
            }
            else
            {
                this.HaveChildren = false;
            }
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach (JFCGridColumn newItem in e.NewItems)
                {
                    newItem.Parent = this;
                }
            }
            else if (e.Action != NotifyCollectionChangedAction.Move)
            {
                if (e.Action == NotifyCollectionChangedAction.Remove)
                {
                    foreach (JFCGridColumn oldItem in e.OldItems)
                    {
                        oldItem.Parent = null;
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Replace)
                {
                    foreach (JFCGridColumn newItem2 in e.NewItems)
                    {
                        newItem2.Parent = this;
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Add)
                {
                    foreach (JFCGridColumn oldItem2 in e.OldItems)
                    {
                        oldItem2.Parent = null;
                    }
                }
            }
        }
    }
}