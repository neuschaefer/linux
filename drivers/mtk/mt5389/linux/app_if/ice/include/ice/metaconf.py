# ice meta-configuration

component.use_type('interface')
component.requires('frost')
component.shares('cabot_shared')

# to enable(1)/disable(None) ICE TCP/IP related functionality
metaconf.INCLUDE_SOCKET = DefineOrNotMetaConfOption('bc', None)
