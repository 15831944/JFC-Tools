using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace WpfApplicationJFCGrid
{
    public class Person : INotifyPropertyChanged
    {
        public Person()
        {

        }

        public Person(string Nom, string Prenom, int Age)
        {
            this.Nom = Nom;
            this.Prenom = Prenom;
            this.Age = Age;
            this.IsChecked = false;
        }

        public Person(string Nom, string Prenom, int Age, bool IsChecked)
        {
            this.Nom = Nom;
            this.Prenom = Prenom;
            this.Age = Age;
            this.IsChecked = IsChecked;
        }

        public string Nom { get; set; }
        public string Prenom { get; set; }
        public int Age { get; set; }
        public bool IsChecked { get; set; }

        private bool isExpande = false;
        public bool IsExpande 
        {
            get { return isExpande; }
            set 
            { 
                isExpande = value;
                OnPropertyChanged(new PropertyChangedEventArgs("IsExpande"));
            }
        }


        public string[] text
        {
            get { return new string[2] { "fred", "alain" }; }
        }
        
        private List<Person> children = new List<Person>();

        public List<Person> Children
        {
            get { return children; }
        }

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;
        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        #endregion

    }
}
