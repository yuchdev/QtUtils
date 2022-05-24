<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" exclude-result-prefixes="xs">
<xsl:output method="text" encoding="UTF-8" indent="yes"/>
  <xsl:template match="TS">
    <xsl:text>Source&#x9;</xsl:text>
    <xsl:text>Translation&#x9;</xsl:text>
    <xsl:text>Comment&#x9;</xsl:text>
    <xsl:text>&#10;</xsl:text>
    <xsl:for-each select="context">
      <xsl:for-each select="message">
        <xsl:value-of select="source" />
        <xsl:text>&#x9;</xsl:text>
        <xsl:value-of select="translation" />
        <xsl:text>&#x9;</xsl:text>
        <xsl:if test="translatorcomment">
          <xsl:value-of select="translatorcomment" />
          <xsl:text>&#x9;</xsl:text>
        </xsl:if>
        <xsl:if test="not(translatorcomment)">
          <xsl:text>&#x9;</xsl:text>
        </xsl:if>
        <xsl:text>&#10;</xsl:text>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>