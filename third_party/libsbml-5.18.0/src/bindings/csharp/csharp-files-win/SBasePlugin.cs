//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace libsbml {

 using System;
 using System.Runtime.InteropServices;

/** 
 * @sbmlpackage{core}
 *
@htmlinclude pkg-marker-core.html Base class for extending SBML objects in packages.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The SBasePlugin class is libSBML's base class for extensions of core SBML
 * component objects.  SBasePlugin defines basic virtual methods for
 * reading/writing/checking additional attributes and/or subobjects; these
 * methods should be overridden by subclasses to implement the necessary
 * features of an extended SBML object.
 *
 * @if clike
 * @section sbaseplugin-howto How to extend SBasePlugin for a package implementation
 *
 * 
 * LibSBML package extensions can extend existing libSBML objects such as Model
 * using SBasePlugin as a base class, to hold attributes and/or subcomponents
 * necessary for the SBML package being implemented.  Package developers must
 * implement an SBasePlugin extended class for each element to be extended
 * (e.g., Model, Reaction, and others) where additional attributes and/or
 * top-level objects of the package extension are directly contained.  The
 * following subsections detail the basic steps necessary to use SBasePlugin
 * for the implementation of a class extension.
 *
 * @subsection sbp-identify 1. Identify the SBML components that need to be extended
 *
 * The specification for a SBML Level&nbsp;3 package will define the
 * attributes and subojects that make up the package constructs.  Those
 * constructs that modify existing SBML components such as Model,
 * Reaction, etc., will be the ones that need to be extended using SBasePlugin.
 *
 * For example, the Layout package makes additions to Model,
 * SpeciesReference, and the <code>&lt;sbml&gt;</code> element (which is
 * represented in libSBML by SBMLDocument).  This means that the Layout
 * package extension in libSBML needs to define extended versions of Model,
 * SpeciesReference and SBMLDocument.  Elements @em other than the SBML
 * document need to be implemented using SBasePlugin; the document component
 * must be implemented using SBMLDocumentPlugin instead.
 *
 *
 * @subsection sbp-implement 2. Create a SBasePlugin subclass for each extended SBML component
 *
 * A new class definition that subclasses SBasePlugin needs to be created for
 * each SBML component to be extended by the package.  For instance, the
 * Layout package needs LayoutModelPlugin and LayoutSpeciesReferencePlugin.
 * (As mentioned above, the Layout class also needs LayoutSBMLDocumentPlugin,
 * but this one is implemented using SBMLDocumentPlugin instead of
 * SBasePlugin.)  Below, we describe in detail the different parts of an
 * SBasePlugin subclass definition.
 *
 * @subsubsection sbp-protected 2.1 Define protected data members
 *
 * Data attributes on each extended class in an SBML package will have one of
 * the data types <code>string</code>, <code>double</code>,
 * <code>int</code>, or <code>bool</code>.  Subelements/subobjects will normally
 * be derived from the ListOf class or from SBase.
 *
 * The additional data members must be properly initialized in the class
 * constructor, and must be properly copied in the copy constructor and
 * assignment operator.  For example, the following data member is defined in
 * the <code>GroupsModelPlugin</code> class (in the file
 * <code>GroupsModelPlugin.h</code>):
 * @code{.cpp}
ListOfGroups mGroups;
@endcode
 *
 * @subsubsection sbp-class-methods 2.2 Override SBasePlugin class-related methods
 *
 * The derived class must override the constructor, copy constructor, assignment
 * operator (<code>operator=</code>) and <code>clone()</code> methods from
 * SBasePlugin.
 *
 *
 * @subsubsection sbp-methods-attribs 2.3 Override SBasePlugin virtual methods for attributes
 *
 * If the extended component is defined by the SBML Level&nbsp;3 package to have
 * attributes, then the extended class definition needs to override the
 * following internal methods on SBasePlugin and provide appropriate
 * implementations:
 *
 * @li <code>addExpectedAttributes(ExpectedAttributes& attributes)</code>: This
 * method should add the attributes that are expected to be found on this kind
 * of extended component in an SBML file or data stream.
 *
 * @li <code>readAttributes(XMLAttributes attributes, ExpectedAttributes&
 * expectedAttributes)</code>: This method should read the attributes
 * expected to be found on this kind of extended component in an SBML file or
 * data stream.
 *
 * @li <code>hasRequiredAttributes()</code>: This method should return @c true
 * if all of the required attribute for this extended component are present on
 * instance of the object.
 *
 * @li <code>writeAttributes(XMLOutputStream stream)</code>: This method should
 * write out the attributes of an extended component.  The implementation should
 * use the different kinds of <code>writeAttribute</code> methods defined by
 * XMLOutputStream to achieve this.
 *
 *
 * @subsubsection sbp-methods-elem 2.4 Override SBasePlugin virtual methods for subcomponents
 *
 * If the extended component is defined by the Level&nbsp;3 package to have
 * subcomponents (i.e., full XML elements rather than mere attributes), then the
 * extended class definition needs to override the following internal
 * SBasePlugin methods and provide appropriate implementations:
 *
 * @li <code>createObject(XMLInputStream stream)</code>: Subclasses must
 * override this method to create, store, and then return an SBML object
 * corresponding to the next XMLToken in the XMLInputStream.  To do this,
 * implementations can use methods like <code>peek()</code> on XMLInputStream to
 * test if the next object in the stream is something expected for the package.
 * For example, LayoutModelPlugin uses <code>peek()</code> to examine the next
 * element in the input stream, then tests that element against the Layout
 * namespace and the element name <code>'listOfLayouts'</code> to see if it's
 * the single subcomponent (ListOfLayouts) permitted on a Model object using the
 * Layout package.  If it is, it returns the appropriate object.
 *
 * @li <code>connectToParent(SBase sbase)</code>: This creates a parent-child
 * relationship between a given extended component and its subcomponent(s).
 *
 * @li <code>setSBMLDocument(SBMLDocument d)</code>: This method should set the
 * parent SBMLDocument object on the subcomponent object instances, so that the
 * subcomponent instances know which SBMLDocument contains them.
 *
 * @li <code>enablePackageInternal(string& pkgURI, string& pkgPrefix,
 * bool flag)</code>: This method should enable or disable the subcomponent
 * based on whether a given XML namespace is active.
 *
 * @li <code>writeElements(XMLOutputStream stream)</code>: This method must be
 * overridden to provide an implementation that will write out the expected
 * subcomponents/subelements to the XML output stream.
 *
 * @li <code>readOtherXML(SBase parentObject, XMLInputStream stream)</code>:
 * This function should be overridden if elements of annotation, notes, MathML
 * content, etc., need to be directly parsed from the given XMLInputStream
 * object.
 *
 * @li <code>hasRequiredElements()</code>: This method should return @c true if
 * a given object contains all the required subcomponents defined by the
 * specification for that SBML Level&nbsp;3 package.
 *
 *
 * @subsubsection sbp-methods-xmlns 2.5 Override SBasePlugin virtual methods for XML namespaces
 *
 * If the package needs to add additional <code>xmlns</code> attributes to
 * declare additional XML namespace URIs, the extended class should override the
 * following method:
 *
 * @li <code>writeXMLNS(XMLOutputStream stream)</code>: This method should
 * write out any additional XML namespaces that might be needed by a package
 * implementation.
 *
 *
 * @subsubsection sbp-methods-hooks 2.6 Implement additional methods as needed
 *
 * Extended component implementations can add whatever additional utility
 * methods are useful for their implementation.
 *
 *
 * @else
 *
 * @section ext-basics Basic principles of SBML package extensions in libSBML
 *
 * 
 * SBML Level&nbsp;3's package structure permits modular extensions to the
 * core SBML format.  In libSBML, support for SBML Level&nbsp;3 packages is
 * provided through optional <em>package extensions</em> that can be plugged
 * into libSBML at the time it is built/compiled.  Users of libSBML can thus
 * choose which extensions are enabled in their software applications.
 *
 * LibSBML defines a number of classes that developers of package extensions
 * can use to implement support for an SBML Level&nbsp;3 package.  These
 * classes make it easier to extend libSBML objects with new attributes
 * and/or subobjects as needed by a particular Level&nbsp;3 package.
 * Three overall categories of classes make up libSBML's facilities for
 * implementing package extensions.  There are (1) classes that serve as base
 * classes meant to be subclassed, (2) template classes meant to be
 * instantiated rather than subclassed, and (3) support classes that provide
 * utility features. A given package implementation for libSBML will take
 * the form of code using these and other libSBML classes, placed in a
 * subdirectory of <code>src/sbml/packages/</code>.
 *
 * The basic libSBML distribution includes a number of package extensions
 * implementing support for officially-endorsed SBML Level&nbsp;3 packages;
 * among these are <em>Flux Balance Constraints</em> ('fbc'),
 * <em>Hierarchical %Model Composition</em> ('comp'), <em>%Layout</em>
 * ('layout'), and <em>Qualitative Models</em> ('qual').  They can serve as
 * working examples for developers working to implement other packages.
 *
 * Extensions in libSBML can currently only be implemented in C++ or C;
 * there is no mechanism to implement them first in languages such as
 * Java or Python.  However, once implemented in C++ or C, language
 * interfaces can be generated semi-automatically using the framework in
 * place in libSBML.  (The approach is based on using <a target='_blank'
 * href='http://www.swig.org'>SWIG</a> and facilities in libSBML's build
 * system.)
 *
 *
 * @endif
 */

public class SBasePlugin : global::System.IDisposable {
	private HandleRef swigCPtr;
	protected bool swigCMemOwn;
	
	internal SBasePlugin(IntPtr cPtr, bool cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr    = new HandleRef(this, cPtr);
	}
	
	internal static HandleRef getCPtr(SBasePlugin obj)
	{
		return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
	}
	
	internal static HandleRef getCPtrAndDisown (SBasePlugin obj)
	{
		HandleRef ptr = new HandleRef(null, IntPtr.Zero);
		
		if (obj != null)
		{
			ptr             = obj.swigCPtr;
			obj.swigCMemOwn = false;
		}
		
		return ptr;
	}

  ~SBasePlugin() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          libsbmlPINVOKE.delete_SBasePlugin(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  
/**
   * Returns the namespace URI of the package to which this plugin object
   * belongs.
   *
   * @return the XML namespace URI of the SBML Level&nbsp;3 package
   * implemented by this libSBML package extension.
   */ public
 string getElementNamespace() {
    string ret = libsbmlPINVOKE.SBasePlugin_getElementNamespace(swigCPtr);
    return ret;
  }

  
/**
   * Returns the XML namespace prefix of the package to which this plugin
   * object belongs.
   *
   * @return the XML namespace prefix of the SBML Level&nbsp;3 package
   * implemented by this libSBML package extension.
   */ public
 string getPrefix() {
    string ret = libsbmlPINVOKE.SBasePlugin_getPrefix(swigCPtr);
    return ret;
  }

  
/**
   * Returns the short-form name of the package to which this plugin
   * object belongs.
   *
   * @return the short-form package name (or nickname) of the SBML package
   * implemented by this package extension.
   */ public
 string getPackageName() {
    string ret = libsbmlPINVOKE.SBasePlugin_getPackageName(swigCPtr);
    return ret;
  }

  
/**
   * Creates and returns a deep copy of this SBasePlugin object.
   *
   * @return the (deep) copy of this SBasePlugin object.
   */ public new
 SBasePlugin clone() {
        SBasePlugin ret = (SBasePlugin) libsbml.DowncastSBasePlugin(libsbmlPINVOKE.SBasePlugin_clone(swigCPtr), true);
        return ret;
}

  
/**
   * Return the first child object found with a given identifier.
   *
   * This method searches all the subobjects under this one, compares their
   * identifiers to @p id, and returns the first one that machines.
   * @if clike It uses SBasePlugin::getAllElements(ElementFilter filter) to
   * get the list of identifiers, so the order in which identifiers are
   * searched is the order in which they appear in the results returned by
   * that method.@endif
   *
   * Normally, <code>SId</code> type identifier values are unique across
   * a model in SBML.  However, in some circumstances they may not be, such
   * as if a model is invalid because of multiple objects having the same
   * identifier.
   *
   * @param id string representing the identifier of the object to find.
   *
   * @return pointer to the first object with the given @p id.
   */ public new
 SBase getElementBySId(string id) {
	SBase ret = (SBase) libsbml.DowncastSBase(libsbmlPINVOKE.SBasePlugin_getElementBySId(swigCPtr, id), false);
	return ret;
}

  
/**
   * Return the first child object found with a given meta identifier.
   *
   * This method searches all the subobjects under this one, compares their
   * meta identifiers to @p metaid, and returns the first one that machines.
   *
   * @param metaid string, the metaid of the object to find.
   *
   * @return pointer to the first object found with the given @p metaid.
   */ public new
 SBase getElementByMetaId(string metaid) {
	SBase ret = (SBase) libsbml.DowncastSBase(libsbmlPINVOKE.SBasePlugin_getElementByMetaId(swigCPtr, metaid), false);
	return ret;
}

  
/** */ /* libsbml-internal */ public new
 void connectToParent(SBase sbase) {
    libsbmlPINVOKE.SBasePlugin_connectToParent(swigCPtr, SBase.getCPtr(sbase));
  }

  
/** */ /* libsbml-internal */ public new
 void enablePackageInternal(string pkgURI, string pkgPrefix, bool flag) {
    libsbmlPINVOKE.SBasePlugin_enablePackageInternal(swigCPtr, pkgURI, pkgPrefix, flag);
  }

  
/** */ /* libsbml-internal */ public new
 bool stripPackage(string pkgPrefix, bool flag) {
    bool ret = libsbmlPINVOKE.SBasePlugin_stripPackage(swigCPtr, pkgPrefix, flag);
    return ret;
  }

  
/**
   * Returns the SBMLDocument object containing this object instance.
   *
   *
 * 
 * LibSBML uses the class SBMLDocument as a top-level container for
 * storing SBML content and data associated with it (such as warnings and
 * error messages).  An SBML model in libSBML is contained inside an
 * SBMLDocument object.  SBMLDocument corresponds roughly to the class
 * <i>SBML</i> defined in the SBML Level&nbsp;3 and Level&nbsp;2
 * specifications, but it does not have a direct correspondence in SBML
 * Level&nbsp;1.  (But, it is created by libSBML no matter whether the
 * model is Level&nbsp;1, Level&nbsp;2 or Level&nbsp;3.)
 *
 *
   *
   * This method allows the caller to obtain the SBMLDocument for the
   * current object.
   *
   * @return the parent SBMLDocument object of this plugin object.
   *
   * @see getParentSBMLObject()
   */ public
 SBMLDocument getSBMLDocument() {
    global::System.IntPtr cPtr = libsbmlPINVOKE.SBasePlugin_getSBMLDocument__SWIG_0(swigCPtr);
    SBMLDocument ret = (cPtr == global::System.IntPtr.Zero) ? null : new SBMLDocument(cPtr, false);
    return ret;
  }

  
/**
   * Returns the XML namespace URI for the package to which this object belongs.
   *
   *
 * 
 * In the XML representation of an SBML document, XML namespaces are used to
 * identify the origin of each XML construct used.  XML namespaces are
 * identified by their unique resource identifiers (URIs).  The core SBML
 * specifications stipulate the namespaces that must be used for core SBML
 * constructs; for example, all XML elements that belong to SBML Level&nbsp;3
 * Version&nbsp;1 Core must be placed in the XML namespace identified by the URI
 * <code>'http://www.sbml.org/sbml/level3/version1/core'</code>.  Individual
 * SBML Level&nbsp;3 packages define their own XML namespaces; for example,
 * all elements belonging to the SBML Level&nbsp;3 %Layout Version&nbsp;1
 * package must be placed in the XML namespace
 * <code>'http://www.sbml.org/sbml/level3/version1/layout/version1/'</code>.
 *
 *
   *
   * This method first looks into the SBMLNamespaces object possessed by the
   * parent SBMLDocument object of the current object.  If this cannot be
   * found, this method returns the result of getElementNamespace().
   *
   * @return a string, the URI of the XML namespace to which this object belongs.
   *
   * @see getPackageName()
   * @see getElementNamespace()
   * @see getSBMLDocument()
   */ public
 string getURI() {
    string ret = libsbmlPINVOKE.SBasePlugin_getURI(swigCPtr);
    return ret;
  }

  
/**
   * Returns the parent object to which this plugin object is connected.
   *
   * @return the parent object of this object.
   */ public
 SBase getParentSBMLObject() {
	SBase ret = (SBase) libsbml.DowncastSBase(libsbmlPINVOKE.SBasePlugin_getParentSBMLObject__SWIG_0(swigCPtr), false);
	return ret;
}

  
/**
   * Sets the XML namespace to which this object belongs.
   *
   *
 * 
 * In the XML representation of an SBML document, XML namespaces are used to
 * identify the origin of each XML construct used.  XML namespaces are
 * identified by their unique resource identifiers (URIs).  The core SBML
 * specifications stipulate the namespaces that must be used for core SBML
 * constructs; for example, all XML elements that belong to SBML Level&nbsp;3
 * Version&nbsp;1 Core must be placed in the XML namespace identified by the URI
 * <code>'http://www.sbml.org/sbml/level3/version1/core'</code>.  Individual
 * SBML Level&nbsp;3 packages define their own XML namespaces; for example,
 * all elements belonging to the SBML Level&nbsp;3 %Layout Version&nbsp;1
 * package must be placed in the XML namespace
 * <code>'http://www.sbml.org/sbml/level3/version1/layout/version1/'</code>.
 *
 *
   *
   * @param uri the URI to assign to this object.
   *
   *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif This particular
 * function only does one thing irrespective of user input or 
 * object state, and thus will only return a single value:
 * @li @link libsbml#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
   *
   * @see getElementNamespace()
   */ public
 int setElementNamespace(string uri) {
    int ret = libsbmlPINVOKE.SBasePlugin_setElementNamespace(swigCPtr, uri);
    return ret;
  }

  
/**
   * Returns the SBML Level of the package extension of this plugin object.
   *
   * @return the SBML Level.
   *
   * @see getVersion()
   */ public
 long getLevel() { return (long)libsbmlPINVOKE.SBasePlugin_getLevel(swigCPtr); }

  
/**
   * Returns the Version within the SBML Level of the package extension of
   * this plugin object.
   *
   * @return the SBML Version.
   *
   * @see getLevel()
   */ public
 long getVersion() { return (long)libsbmlPINVOKE.SBasePlugin_getVersion(swigCPtr); }

  
/**
   * Returns the package version of the package extension of this plugin
   * object.
   *
   * @return the package version of the package extension of this plugin
   * object.
   *
   * @see getLevel()
   * @see getVersion()
   */ public
 long getPackageVersion() { return (long)libsbmlPINVOKE.SBasePlugin_getPackageVersion(swigCPtr); }

  
/** */ /* libsbml-internal */ public new
 void replaceSIDWithFunction(string id, ASTNode function) {
    libsbmlPINVOKE.SBasePlugin_replaceSIDWithFunction(swigCPtr, id, ASTNode.getCPtr(function));
  }

  
/** */ /* libsbml-internal */ public new
 void divideAssignmentsToSIdByFunction(string id, ASTNode function) {
    libsbmlPINVOKE.SBasePlugin_divideAssignmentsToSIdByFunction(swigCPtr, id, ASTNode.getCPtr(function));
  }

  
/** */ /* libsbml-internal */ public new
 void multiplyAssignmentsToSIdByFunction(string id, ASTNode function) {
    libsbmlPINVOKE.SBasePlugin_multiplyAssignmentsToSIdByFunction(swigCPtr, id, ASTNode.getCPtr(function));
  }

  
/** */ /* libsbml-internal */ public new
 bool hasIdentifierBeginningWith(string prefix) {
    bool ret = libsbmlPINVOKE.SBasePlugin_hasIdentifierBeginningWith(swigCPtr, prefix);
    return ret;
  }

  
/** */ /* libsbml-internal */ public new
 int prependStringToAllIdentifiers(string prefix) {
    int ret = libsbmlPINVOKE.SBasePlugin_prependStringToAllIdentifiers(swigCPtr, prefix);
    return ret;
  }

  
/**
   *
 * Replaces all uses of a given @c SIdRef type attribute value with another
 * value.
 *
 *
 * 

 * In SBML, object identifiers are of a data type called <code>SId</code>.
 * In SBML Level&nbsp;3, an explicit data type called <code>SIdRef</code> was
 * introduced for attribute values that refer to <code>SId</code> values; in
 * previous Levels of SBML, this data type did not exist and attributes were
 * simply described to as 'referring to an identifier', but the effective
 * data type was the same as <code>SIdRef</code> in Level&nbsp;3.  These and
 * other methods of libSBML refer to the type <code>SIdRef</code> for all
 * Levels of SBML, even if the corresponding SBML specification did not
 * explicitly name the data type.
 *
 *
 *
 * This method works by looking at all attributes and (if appropriate)
 * mathematical formulas in MathML content, comparing the referenced
 * identifiers to the value of @p oldid.  If any matches are found, the
 * matching values are replaced with @p newid.  The method does @em not
 * descend into child elements.
 *
 * @param oldid the old identifier.
 * @param newid the new identifier.
 *
 *
   */ public new
 void renameSIdRefs(string oldid, string newid) {
    libsbmlPINVOKE.SBasePlugin_renameSIdRefs(swigCPtr, oldid, newid);
  }

  
/**
   *
 * Replaces all uses of a given meta identifier attribute value with
 * another value.
 *
 *
 * 
 * In SBML, object 'meta' identifiers are of the XML data type <code>ID</code>;
 * the SBML object attribute itself is typically named <code>metaid</code>.  All
 * attributes that hold values <em>referring</em> to values of type
 * <code>ID</code> are of the XML data type <code>IDREF</code>.  They are also
 * sometimes informally referred to as 'metaid refs', in analogy to the
 * SBML-defined type <code>SIdRef</code>.
 *
 *
 *
 * This method works by looking at all meta-identifier attribute values,
 * comparing the identifiers to the value of @p oldid.  If any matches are
 * found, the matching identifiers are replaced with @p newid.  The method
 * does @em not descend into child elements.
 *
 * @param oldid the old identifier.
 * @param newid the new identifier.
 *
 *
   */ public new
 void renameMetaIdRefs(string oldid, string newid) {
    libsbmlPINVOKE.SBasePlugin_renameMetaIdRefs(swigCPtr, oldid, newid);
  }

  
/**
   *
 * Replaces all uses of a given @c UnitSIdRef type attribute value with
 * another value.
 *
 *
 * 
 * In SBML, unit definitions have identifiers of type <code>UnitSId</code>.  In
 * SBML Level&nbsp;3, an explicit data type called <code>UnitSIdRef</code> was
 * introduced for attribute values that refer to <code>UnitSId</code> values; in
 * previous Levels of SBML, this data type did not exist and attributes were
 * simply described to as 'referring to a unit identifier', but the effective
 * data type was the same as <code>UnitSIdRef</code> in Level&nbsp;3.  These and
 * other methods of libSBML refer to the type <code>UnitSIdRef</code> for all
 * Levels of SBML, even if the corresponding SBML specification did not
 * explicitly name the data type.
 *
 *
 *
 * This method works by looking at all unit identifier attribute values
 * (including, if appropriate, inside mathematical formulas), comparing the
 * referenced unit identifiers to the value of @p oldid.  If any matches
 * are found, the matching values are replaced with @p newid.  The method
 * does @em not descend into child elements.
 *
 * @param oldid the old identifier.
 * @param newid the new identifier.
 *
 *
   */ public new
 void renameUnitSIdRefs(string oldid, string newid) {
    libsbmlPINVOKE.SBasePlugin_renameUnitSIdRefs(swigCPtr, oldid, newid);
  }

  
/** */ /* libsbml-internal */ public new
 int transformIdentifiers(IdentifierTransformer sidTransformer) {
    int ret = libsbmlPINVOKE.SBasePlugin_transformIdentifiers(swigCPtr, IdentifierTransformer.getCPtr(sidTransformer));
    return ret;
  }

  
/** */ /* libsbml-internal */ public
 long getLine() { return (long)libsbmlPINVOKE.SBasePlugin_getLine(swigCPtr); }

  
/** */ /* libsbml-internal */ public
 long getColumn() { return (long)libsbmlPINVOKE.SBasePlugin_getColumn(swigCPtr); }

  
/** */ /* libsbml-internal */ public new
 SBMLNamespaces getSBMLNamespaces() {
	SBMLNamespaces ret
	    = (SBMLNamespaces) libsbml.DowncastSBMLNamespaces(libsbmlPINVOKE.SBasePlugin_getSBMLNamespaces(swigCPtr), false);
	return ret;
}

  
/** */ /* libsbml-internal */ public new
 void logUnknownElement(string element, long sbmlLevel, long sbmlVersion, long pkgVersion) {
    libsbmlPINVOKE.SBasePlugin_logUnknownElement(swigCPtr, element, sbmlLevel, sbmlVersion, pkgVersion);
  }

  
/** */ public new
 bool isValidTypeForList(SBase item) {
    bool ret = libsbmlPINVOKE.SBasePlugin_isValidTypeForList(swigCPtr, SBase.getCPtrAndDisown(item));
    return ret;
  }

  
/** */ /* libsbml-internal */ public
 SBMLExtension getSBMLExtension() {
        SBMLExtension ret
	    = (SBMLExtension) libsbml.DowncastExtension(libsbmlPINVOKE.SBasePlugin_getSBMLExtension(swigCPtr), false);
        return ret;
}

  
/** */ /* libsbml-internal */ public new
 void updateSBMLNamespace(string package, long level, long version) {
    libsbmlPINVOKE.SBasePlugin_updateSBMLNamespace(swigCPtr, package, level, version);
  }

  public SBaseList getListOfAllElements(ElementFilter filter) {
    global::System.IntPtr cPtr = libsbmlPINVOKE.SBasePlugin_getListOfAllElements__SWIG_0(swigCPtr, ElementFilter.getCPtr(filter));
    SBaseList ret = (cPtr == global::System.IntPtr.Zero) ? null : new SBaseList(cPtr, false);
    return ret;
  }

  public SBaseList getListOfAllElements() {
    global::System.IntPtr cPtr = libsbmlPINVOKE.SBasePlugin_getListOfAllElements__SWIG_1(swigCPtr);
    SBaseList ret = (cPtr == global::System.IntPtr.Zero) ? null : new SBaseList(cPtr, false);
    return ret;
  }

}

}
