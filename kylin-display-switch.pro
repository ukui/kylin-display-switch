TEMPLATE = subdirs

CONFIG += ordered


SUBDIRS =  \
          daemon \
          kds \
          mkt \
    registerdbus \
    daemon2

TRANSLATIONS +=  \
                shared/i18n/zh_CN.ts \

