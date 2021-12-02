/** @addtogroup xml xml
 *  @ingroup cmn
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 * @depends basic_types.h
 * @depends binstr_api.h
 * @depends time_api.h
 */

#ifndef XMLP_API_H
#define XMLP_API_H

NP_BEGIN_EXTERN_C

/*===========================================================================*/
/** @file
 * @brief XML processing library
 *
 * dependency base,b64
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 * @date   29 Nov, 2004 last modified by n-goto
 */
/*===========================================================================*/

/*---------------------------------------------------------------------------*
 * macro definitions
 *---------------------------------------------------------------------------*/

/**
 * @brief Error code of XML library
 */
#define ERR_XML_NOTIMPL         (ERR_XML_BASE|1)
#define ERR_XML_BROKENOBJECT        (ERR_XML_BASE|2)
#define ERR_XML_PARSEFAIL       (ERR_XML_BASE|3)
#define ERR_XML_UNEXPECTEDEOF       (ERR_XML_BASE|4)
#define ERR_XML_MISMATCHTAG     (ERR_XML_BASE|5)
#define ERR_XML_MISMATCHNODE        (ERR_XML_BASE|6)
#define ERR_XML_ILLEGALXPATH        (ERR_XML_BASE|7)
#define ERR_XML_NOTFOUND        (ERR_XML_BASE|8)
#define ERR_XML_ILLEGALNODE     (ERR_XML_BASE|9)
#define ERR_XML_TOOMANYNODES        (ERR_XML_BASE|10)

/**
 * @brief node-type definitions
 */
#define XML_NODE_TYPE_ROOT      0
#define XML_NODE_TYPE_NAMESPACE 1
#define XML_NODE_TYPE_ATTRIBUTE 2
#define XML_NODE_TYPE_ELEMENT   3
#define XML_NODE_TYPE_TEXT      4

/**
 * @brief node-type definitions
 */
#define XML_CONTENT_TYPE_TEXT    0
#define XML_CONTENT_TYPE_ELEMENT 1
#define XML_CONTENT_TYPE_BINSTR  2
#define XML_CONTENT_TYPE_INTEGER 3
#define XML_CONTENT_TYPE_TIME    4
#define XML_CONTENT_TYPE_UNSIGNED_MULTIBYTE_INTEGER 5 /* An unsigned mutlibyte integer in Big Endian (e.g., used for crlnumbers) */

/**
 * @brief XML Namespaces/Algorithms Definitions
 */
#define XML_NS_XMLSCHEMA "http://www.w3.org/2001/XMLSchema-instance"
#define XML_ALGO_ENVSIG "http://www.w3.org/2000/09/xmldsig#enveloped-signature"
#define XML_ALGO_EXCLC14N "http://www.w3.org/2001/10/xml-exc-c14n#"
#define XML_ALGO_HMACSHA1 "http://www.w3.org/2000/09/xmldsig#hmac-sha1"
#define XML_ALGO_HMACSHA256 "http://www.w3.org/2001/04/xmldsig-more#hmac-sha256"
#define XML_ALGO_RSASHA1 "http://www.w3.org/2000/09/xmldsig#rsa-sha1"
#define XML_ALGO_RSASHA256 "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"
#define XML_ALGO_RSAV15 "http://www.w3.org/2001/04/xmlenc#rsa-1_5"
#define XML_ALGO_RSAOAEP_MGF1P "http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p"
#define XML_ALGO_AES128CBC "http://www.w3.org/2001/04/xmlenc#aes128-cbc"
#define XML_ALGO_SHA1 "http://www.w3.org/2000/09/xmldsig#sha1"
#define XML_ALGO_SHA256 "http://www.w3.org/2001/04/xmlenc#sha256"
#define XML_ALGO_RSA XML_ALGO_RSAV15
#define XML_ALGO_AES XML_ALGO_AES128CBC
#define XML_ALGO_STARFISH "http://marlin-drm.com/starfish/algorithmID/1.0"

/*---------------------------------------------------------------------------*
 * type definitions
 *---------------------------------------------------------------------------*/

/**
 * XML node
 */
typedef struct xml_node_tag {
    int32_t ref;  /**< reference counter */
    int32_t type; /**< node type: ::XML_NODE_TYPE_ROOT, ::XML_NODE_TYPE_NAMESPACE,
                              ::XML_NODE_TYPE_ATTRIBUTE, ::XML_NODE_TYPE_ELEMENT,
                              ::XML_NODE_TYPE_TEXT */
    union {
        void *obj; /**< see ::xml_l_node_t */
    } c;
} xml_node_t;

/**
 * Vector of children nodes
 */
typedef struct xml_node_vec_tag {
    int32_t ref;         /**< reference counter */
    int32_t size;        /**< number of children nodes */
    xml_node_t *node[1]; /**< array of children nodes */
} xml_node_vec_t;

/**
 * Attribute data
 */
typedef struct {
    char *name;  /**< attribute name */
    char *value; /**< attribute value */
} xml_attribute_data_t;

/**
 * Content of an XML node
 */
typedef struct {
    int32_t type; /**< content type: ::XML_CONTENT_TYPE_INTEGER, ::XML_CONTENT_TYPE_TIME,
                                 ::XML_CONTENT_TYPE_TEXT, ::XML_CONTENT_TYPE_BINSTR,
                                 ::XML_CONTENT_TYPE_ELEMENT */
    union {
        int32_t      i;     /**< applicable if type is XML_CONTENT_TYPE_INTEGER */
        time_date_t  *time; /**< applicable if type is XML_CONTENT_TYPE_TIME */
        char         *text; /**< applicable if type is XML_CONTENT_TYPE_TEXT */
        binstr_t     *str;  /**< applicable if type is XML_CONTENT_TYPE_BINSTR */
        xml_node_t   *node; /**< applicable if type is XML_CONTENT_TYPE_ELEMENT */
    } c;
} xml_content_data_t;

/**
 * Parse a complete XML document
 *
 * @param[in]  in_context    memory context
 * @param[in]  in_xmltextlen length of the XML document to parse
 * @param[in]  in_xmltext    XML document to parse
 * @param[out] out_xmlnode   root XML node
 * @return ::ERR_OK
 * @return ::npi_xml_l_node_create
 * @return ::npi_xml_l_document_create
 * @return ::npi_xml_l_ParsePrologue
 * @return ::l_ParseElement
 */
extern retcode_t
npi_xml_ParseBuf(npi_cmncontext_t in_context,
                 np_size_t        in_xmltextlen,
                 const u_int8_t  *in_xmltext,
                 xml_node_t     **out_xmlnode);
#define npi_xml_Parse(c,t,n) npi_xml_ParseBuf(c, npi_binstr_Len(t), npi_binstr_Buf(t), n)

/**
 * Parse XML text and insert it into a parent XML node
 *
 * @note New nodes inherit namespaces from the parent node ::io_parent
 *
 * @param[in]     in_context    memory context
 * @param[in]     in_xmltextlen length of the XML text to parse
 * @param[in]     in_xmltext    XML text to parse, not a complete XML document, only the following pattern is accepted
 * @verbatim content ::= CharData? ((element | Reference | CDSect | PI | Comment) CharData?)* @endverbatim
 * @param[in,out] io_parent     parent node to which the parsed XML will be inserted
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_ParseContent
 */
extern retcode_t
npi_xml_ParseAndInsert(npi_cmncontext_t in_context,
                       np_size_t        in_xmltextlen,
                       const u_int8_t  *in_xmltext,
                       xml_node_t      *io_parent);

/**
 * Parse an XML document and insert it instead of another node into a parent XML node
 *
 * @note New nodes inherit namespaces from the parent node ::io_parent
 * @note If the XML document to parse has a prolog, it will be skipped
 *
 * @param[in]     in_context    memory context
 * @param[in]     in_xmltextlen length of XML document to parse
 * @param[in]     in_xmltext    XML document to parse
 * @param[in,out] io_victim     node (child of parent) to be replaced by the parsed XML
 * @param[in,out] io_parent     parent node of ::io_victim
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_ParsePrologue
 * @return ::npi_xml_l_ParseContent
 */
extern retcode_t
npi_xml_ParseAndReplace(npi_cmncontext_t in_context,
                        np_size_t        in_xmltextlen,
                        const u_int8_t  *in_xmltext,
                        xml_node_t      *io_victim,
                        xml_node_t      *io_parent);

/**
 * Parse an signed integer in base 10
 *
 * @note Independent from XML parsing
 *
 * @param[in]  in_context    memory context
 * @param[in]  in_xmltextlen length of XML text to parse
 * @param[in]  in_xmltext    XML text to parse
 * @param[out] out_i         parsed integer
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_XML_PARSEFAIL
 */
extern retcode_t
npi_xml_ParseInteger(npi_cmncontext_t in_context,
                     np_size_t        in_xmltextlen,
                     const u_int8_t  *in_xmltext,
                     int32_t         *out_i);

/**
 * Parse a DateTime
 *
 * Accepted format: @verbatim '-'? yyyy '-' mm '-' dd 'T' hh ':' mm ':' ss ('.' s+)? (zzzzzz)? @endverbatim
 * @specref http://www.w3.org/TR/xmlschema-2/#dateTime
 * @todo check compliance
 *
 * @note Independent from XML parsing
 *
 * @param[in]  in_context    memory context
 * @param[in]  in_xmltextlen length of XML text to parse
 * @param[in]  in_xmltext    XML text to parse
 * @param[out] out_i         parsed date
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_XML_PARSEFAIL
 */
extern retcode_t
npi_xml_ParseDateTime(npi_cmncontext_t in_context,
                      np_size_t        in_xmltextlen,
                      const u_int8_t  *in_xmltext,
                      time_date_t     *out_time);


/**
 * Find XML nodes from an xpath
 *
 * @specref http://www.w3.org/TR/xpath
 * @todo check level of compliance with xpath
 *
 * @param[in]  in_context   memory context
 * @param[in]  in_xmlnode   node from which to start finding
 * @param[in]  in_xpath     xpath of the node(s) to find
 * @param[in]  in_namespace namespace of the node(s) to find
 * @param[out] out_xmlnodes vector of found XML nodes
 * @return ::ERR_OK
 * @return ::ERR_XML_MISMATCHNODE
 * @return ::ERR_XML_NOTFOUND
 * @return ::l_FindOneStep
 */
extern retcode_t
npi_xml_Find(npi_cmncontext_t in_context,
             xml_node_t      *in_xmlnode,
             char            *in_xpath,
             char            *in_namespace,
             xml_node_vec_t **out_xmlnodes);

/**
 * Find (and expect only) 1 XML node from an xpath
 *
 * @param[in]  in_context   memory context
 * @param[in]  in_xmlnode   node from which to start finding
 * @param[in]  in_xpath     xpath of the node to find
 * @param[in]  in_namespace namespace of the node to find
 * @param[out] out_xmlnode  the found XML node
 * @return ::ERR_OK
 * @return ::ERR_XML_TOOMANYNODES
 * @return ::npi_xml_Find
 */
extern retcode_t
npi_xml_FindOne(npi_cmncontext_t in_context,
                xml_node_t      *in_xmlnode,
                char            *in_xpath,
                char            *in_namespace,
                xml_node_t     **out_xmlnode);

/**
 * Get the element of a given node
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_xmlnode  XML node from which to get the element
 * @param[out] out_topnode New XML node (of type ::XML_NODE_TYPE_ELEMENT) that share the element of the given node
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_node_create_element
 */
extern retcode_t
npi_xml_GetTopElement(npi_cmncontext_t in_context,
                      xml_node_t      *in_xmlnode,
                      xml_node_t     **out_topnode);

/**
 * Get the value of an attribute of an XML node
 *
 * @param[in]  in_context    memory context
 * @param[in]  in_xmlnode    XML node from which to get the value of an attribute
 * @param[in]  in_namespace  namespace of the attribute to get
 * @param[in]  in_attrname   name of the attribute to get
 * @param[out] out_attrvalue value of the requested attribute
 * @return ::ERR_OK
 * @return ::ERR_XML_NOTFOUND
 * @return ::ERR_NOMEM
 */
extern retcode_t
npi_xml_GetAttr(npi_cmncontext_t in_context,
                xml_node_t      *in_xmlnode,
                char            *in_namespace,
                char            *in_attrname,
                binstr_t       **out_attrvalue);

/**
 * Get the content of an XML node
 *
 * If XML node type is
 * @li ::XML_NODE_TYPE_ELEMENT, the returned content is the first content of type XML_L_CONTENT_TYPE_TEXT
 * @li ::XML_NODE_TYPE_ATTRIBUTE, the returned content is the value of the attribute
 * @li ::XML_NODE_TYPE_NAMESPACE, the returned content is the value of the namespace
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_xmlnode  XML node from which to get the content
 * @param[out] out_content requested content
 * @return ::ERR_OK
 * @return ::ERR_XML_NOTFOUND
 * @return ::ERR_NOMEM
 */
extern retcode_t
npi_xml_GetContent(npi_cmncontext_t in_context,
                   xml_node_t      *in_xmlnode,
                   binstr_t       **out_content);

/**
 * Get the content of an XML node
 *
 * If XML node type is
 * @li ::XML_NODE_TYPE_ELEMENT, the returned name is the name of the node
 * @li ::XML_NODE_TYPE_ATTRIBUTE, the returned name is the name of the attribute
 * @li ::XML_NODE_TYPE_NAMESPACE, the returned name is the name of the namespace
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_xmlnode  XML node from which to get the name
 * @param[out] out_content name of the node
 * @return ::ERR_OK
 * @return ::ERR_XML_NOTFOUND
 * @return ::ERR_NOMEM
 */
extern retcode_t
npi_xml_GetName(npi_cmncontext_t in_context,
                xml_node_t      *in_xmlnode,
                binstr_t       **out_content);

/**
 * Canonicalize an XML node (including the sub nodes recursively)
 *
 * @specref XML Canonicalization (C14N) http://www.w3.org/TR/xml-c14n
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_xmlnode  XML node to canonicalize
 * @param[out] out_xmltext canonicalized XML node
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_XML_NOTIMPL
 * @return ::l_OutputDocumentExcl
 * @return ::l_OutputElementExcl
 * @return ::l_OutputText
 */
extern retcode_t
npi_xml_ExclC14N(npi_cmncontext_t in_context,
                 xml_node_t      *in_xmlnode,
                 binstr_t       **out_xmltext);

/**
 * Serialize an XML node (including the sub nodes recursively)
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_xmlnode  XML node to serialize
 * @param[out] out_xmltext serialized XML node
 */
extern retcode_t
npi_xml_ToString(npi_cmncontext_t in_context,
                 xml_node_t      *in_xmlnode,
                 binstr_t       **out_xmltext);

/**
 * Return a (shared) copy of an XML node
 *
 * @note This is not a copy by value, only the reference counter of the XML node is increased
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    node to "copy"
 * @return ::io_node
 */
extern xml_node_t *
npi_xml_CopyNode(npi_cmncontext_t in_context,
                 xml_node_t      *io_node);

/**
 * Free an XML node (including the sub nodes recursively)
 *
 * @note The reference coutner of an XML node is first decreased, if it becomes 0 the node is effectively freed
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    XML node to free
 * @return ::ERR_OK
 */
extern retcode_t
npi_xml_FreeNode(npi_cmncontext_t in_context,
                 xml_node_t      *io_node);

/**
 * Free a vector of nodes (including the sub nodes recursively)
 *
 * @note The reference coutner of the vector of XML nodes is first decreased,
 *       if it becomes 0 the nodes of the vector are freed (::npi_xml_FreeNode)
 *       and the vector is effectively freed
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_nodevec vector of nodes to free
 * @return ::ERR_OK
 */
extern retcode_t
npi_xml_FreeNodeVec(npi_cmncontext_t in_context,
                    xml_node_vec_t  *io_nodevec);

/**
 * Get the number of items in a vector of nodes
 *
 * @param[in] nodevec vector of nodes
 * @return the number of items
 */
#define npi_xml_NodeVecSize(nodevec) ((nodevec)->size)

/**
 * Get the i_th item of a vector of nodes
 *
 * @param[in] nodevec vector of nodes
 * @param[in] index of the item to get
 * @return the i_th item of a vector of nodes
 */
#define npi_xml_NodeVecItem(nodevec,idx) ((nodevec)->node[idx])

/**
 * Compose an XML node from a name, namespace, attributes and contents
 *
 * @note Namespaces are automatically added to sub contents
 *
 * The function makes an XML node that, if serialized, would be as follows: @verbatim
<in_namespacetag:in_name xmlns:in_namespacetag=in_namespaceuri in_attrs...>
  io_conts
</in_namespacetag:in_name>
@endverbatim
 *
 * @param[in]     in_context      memory context
 * @param[in]     in_name         name (tag) of the node to compose
 * @param[in]     in_namespacetag namespace of the tag
 * @param[in]     in_namespaceuri uri of the namespace (xmlns:in_namespacetag=in_namespaceuri)
 * @param[in]     in_nattrs       number of attributes of the tag
 * @param[in]     in_attrs        vector of attributes
 * @param[in]     in_nconts       number of sub contents
 * @param[in,out] io_conts        vector of sub contents
 * @param[in]     in_freef        flag: if not 0, contents of ::io_conts are freed after the operation
 * @param[out]    out_node        the created XML node (type is ::XML_NODE_TYPE_ELEMENT)
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::npi_xml_l_element_create
 * @return ::npi_xml_l_attribute_vec_create
 * @return ::npi_xml_l_attribute_create
 * @return ::l_AddNamespacesRec
 * @return ::npi_xml_l_content_vec_create
 * @return ::npi_xml_l_content_create
 * @return ::npi_xml_l_conv_itos
 * @return ::npi_xml_l_conv_ttos
 * @return ::npi_xml_l_node_create
 */
extern retcode_t
npi_xml_ComposeElement(npi_cmncontext_t      in_context,
                       char                 *in_name,
                       char                 *in_namespacetag,
                       char                 *in_namespaceuri,
                       int32_t               in_nattrs,
                       xml_attribute_data_t *in_attrs,
                       int32_t               in_nconts,
                       xml_content_data_t   *in_conts,
                       int32_t               in_freef,
                       xml_node_t          **out_node);

/**
 * Add an attribute to an XML node
 *
 * @note if the attribute is a namespace (strating with xmlns),
 *       it will be added to the vector of namespaces of the XML node
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    node to which to add an attribute
 * @param[in]     in_name    name of the attribute to add
 * @param[in]     in_value   value of the attribute to add
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::ERR_NOMEM
 * @return ::npi_xml_l_attribute_create
 * @return ::npi_xml_l_attribute_vec_create
 */
extern retcode_t
npi_xml_AddAttribute(npi_cmncontext_t in_context,
                     xml_node_t      *io_node,
                     char            *in_name,
                     char            *in_value);

/**
 * Add a text content to an XML node
 *
 * @note The text is wrapped into a content object (type ::XML_CONTENT_TYPE_TEXT)
 *       and added to the vector of contents of the XML node
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    node to which to add a text
 * @param[in]     in_text    text to add
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::ERR_NOMEM
 * @return ::npi_xml_l_content_create
 * @return ::npi_xml_l_content_vec_create
 */
extern retcode_t
npi_xml_AddText(npi_cmncontext_t in_context,
                xml_node_t      *io_node,
                char            *in_text);

/*
 * @brief This method replaces whatever text might be present in the input xml node
 *        with new text passed in.
 *
 * If the current node has no contents, then a new content item will be made and filled 
 * in with the input text.
 * If the current node already has contents, then all TEXT content items will be removed
 * and a new content item with the input text will be appended to the content item list.
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    node that will receive new text
 * @param[in]     in_text    new text for io_node
 *
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::npi_xml_AddText
 */
extern retcode_t
npi_xml_ReplaceText(npi_cmncontext_t     in_context,
                    xml_node_t          *io_node,
                    char                *in_text);

/**
 * Add a child XML node to an XML node
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_node    node to which to add a child node (type must be ::XML_NODE_TYPE_ELEMENT)
 * @param[in]     io_child   node to add (type must be ::XML_NODE_TYPE_ELEMENT)
 * @param[in]     in_freef   flag: if not 0, ::io_child is freed after the operation
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_content_create
 * @return ::npi_xml_l_AddContents
 */
extern retcode_t
npi_xml_AddElement(npi_cmncontext_t in_context,
                   xml_node_t      *io_node,
                   xml_node_t      *io_child,
                   int32_t          in_freef);

/**
 * Serialize an element node into an XML document
 *
 * @param[in]  in_context  memory context
 * @param[in]  in_node     node to output (type must be ::XML_NODE_TYPE_ELEMENT)
 * @param[in]  in_version  version to put in the XML prolog
 * @param[in]  in_encoding encoding to put int the XML prolog <?xml version='in_version' encoding='in_encoding' ?>
 * @param[out] out_str     XML document (serialized XML node)
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::ERR_NOMEM
 * @return ::npi_xml_ToString
 */
extern retcode_t
npi_xml_OutputComposedNode(npi_cmncontext_t in_context,
                           xml_node_t      *in_node,
                           char            *in_version,
                           char            *in_encoding,
                           binstr_t       **out_str);

/**
 * Serialize an array of element nodes
 *
 * @param[in]  in_context memory context
 * @param[in]  in_count   number of nodes in the array ::in_nodes
 * @param[in]  in_nodes   array of nodes to serialize (types must be ::XML_NODE_TYPE_ELEMENT)
 * @param[out] out_str    XML document (serialized XML nodes)
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::ERR_NOMEM
 * @return ::npi_xml_ToString
 */
extern retcode_t
npi_xml_OutputComposedNodes(npi_cmncontext_t in_context,
                            int32_t          in_count,
                            xml_node_t     **in_nodes,
                            binstr_t       **out_str);

/**
 * Remove a child XML node from a parent XML node
 *
 * @param[in]     in_context memory context
 * @param[in,out] io_parent  parent from which to remove the child node (type must be ::XML_NODE_TYPE_ELEMENT)
 * @param[in,out] io_child   child node to remove (type must be ::XML_NODE_TYPE_ELEMENT)
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_RemoveElement
 */
extern retcode_t
npi_xml_RemoveContent(npi_cmncontext_t in_context,
                      xml_node_t *io_parent,
                      xml_node_t *io_child);

/**
 * Remove a child XML node from a parent XML node
 *
 * @param[in]       in_context          memory context
 * @param[in,out]   io_root_node        root node 
 * @param[in,out]   out_element_node    child element node of root
 * @return ::ERR_OK
 * @return ::ERR_XML_ILLEGALNODE
 * @return ::npi_xml_l_node_create_element
 */                      
extern retcode_t
npi_xml_GetElementFromRoot(npi_cmncontext_t in_context,
                           xml_node_t      *io_root_node, 
                           xml_node_t     **out_element_node );

                      
NP_END_EXTERN_C

#endif /* XMLP_API_H */
/** @} */
