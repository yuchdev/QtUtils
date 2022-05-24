<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
  <xsl:template match="TS">
    <table align="left" style="font-family:arial;font-size:12px" border="2">
      <tr>
        <th>Source</th>
        <th>Translation</th>
        <th>Comment</th>
      </tr>
      <xsl:for-each select="context">
        <xsl:for-each select="message">
          <tr align="left">
            <td align="left">
              <xsl:value-of select="source" />
            </td>
            <td align="left">
              <xsl:value-of select="translation" />
            </td>
            <xsl:if test="translatorcomment">
              <td align="left">
                <xsl:value-of select="translatorcomment" />
              </td>
            </xsl:if>
            <xsl:if test="not(translatorcomment)">
              <td></td>
            </xsl:if>
          </tr>
        </xsl:for-each>
      </xsl:for-each>
    </table>
  </xsl:template>
</xsl:stylesheet>