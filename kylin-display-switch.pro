TEMPLATE = subdirs

CONFIG += ordered


SUBDIRS =  \
          daemon \
          kds \
          mkt \
    registerdbus

TRANSLATIONS +=  \
                shared/i18n/zh_CN.ts \

