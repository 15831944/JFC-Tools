﻿//------------------------------------------------------------------------------
// <auto-generated>
//     Ce code a été généré par un outil.
//     Version du runtime :4.0.30319.18052
//
//     Les modifications apportées à ce fichier peuvent provoquer un comportement incorrect et seront perdues si
//     le code est régénéré.
// </auto-generated>
//------------------------------------------------------------------------------

using System.Xml.Serialization;

// 
// Ce code source a été automatiquement généré par xsd, Version=4.0.30319.1.
// 


/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.0.30319.1")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://schemas.openxmlformats.org/package/2006/relationships")]
[System.Xml.Serialization.XmlRootAttribute(Namespace="http://schemas.openxmlformats.org/package/2006/relationships", IsNullable=false)]
public partial class Relationships {
    
    private RelationshipsRelationship[] relationshipField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("Relationship")]
    public RelationshipsRelationship[] Relationship {
        get {
            return this.relationshipField;
        }
        set {
            this.relationshipField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.0.30319.1")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://schemas.openxmlformats.org/package/2006/relationships")]
public partial class RelationshipsRelationship {
    
    private string typeField;
    
    private string targetField;
    
    private string idField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string Type {
        get {
            return this.typeField;
        }
        set {
            this.typeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string Target {
        get {
            return this.targetField;
        }
        set {
            this.targetField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string Id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
}
