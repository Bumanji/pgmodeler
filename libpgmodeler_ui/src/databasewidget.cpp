#include "databasewidget.h"

DatabaseWidget::DatabaseWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DATABASE)
{
	map<QString, vector<QWidget *> > fields_map;
	QFrame *frame=NULL;
	QLocale loc;
	QStringList loc_list, encodings;
	unsigned i,i1;

	Ui_DatabaseWidget::setupUi(this);

	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	configureFormLayout(database_grid, OBJ_DATABASE);

	//Defines the fields used on specific PostgreSQL versions
	fields_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(connlim_lbl);
	fields_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(lccollate_lbl);
	fields_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(lcctype_lbl);

	frame=generateVersionWarningFrame(fields_map);
	database_grid->addWidget(frame, database_grid->count()+1, 0, 1, 0);
	frame->setParent(this);

	parent_form->setMinimumWidth(530);
	parent_form->setMinimumHeight(420);
	parent_form->setMaximumHeight(420);

	//Configures the encoding combobox
	EncodingType::getTypes(encodings);
	encoding_cmb->addItems(encodings);

	//Configures the localizations combobox
	for(i=QLocale::C; i <= QLocale::Chewa; i++)
	{
		for(i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
		{
			loc=QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1));
			loc_list.append(loc.name());
		}
	}


	loc_list.removeDuplicates();
	loc_list.sort();

	lccollate_cmb->addItem(trUtf8("Default"));
	lcctype_cmb->addItem(trUtf8("Default"));
	lccollate_cmb->addItems(loc_list);
	lcctype_cmb->addItems(loc_list);
}

void DatabaseWidget::setAttributes(DatabaseModel *model)
{
	if(model)
	{
		int idx;

		connlim_sb->setValue(model->getConnectionLimit());
		templatedb_edt->setText(Utf8String::create(model->getTemplateDB()));
		author_edt->setText(Utf8String::create(model->getAuthor()));

		idx=encoding_cmb->findText(~model->getEncoding());
		if(idx < 0) idx=0;
		encoding_cmb->setCurrentIndex(idx);

		idx=lccollate_cmb->findText(model->getLocalization(LC_COLLATE));
		if(idx < 0) idx=0;
		lccollate_cmb->setCurrentIndex(idx);

		idx=lcctype_cmb->findText(model->getLocalization(LC_CTYPE));
		if(idx < 0) idx=0;
		lcctype_cmb->setCurrentIndex(idx);

		BaseObjectWidget::setAttributes(model, model, NULL);
	}
}

void DatabaseWidget::applyConfiguration(void)
{
	try
	{
		//Apply the basic configurations
		BaseObjectWidget::applyConfiguration();

		model->setAuthor(author_edt->text().toUtf8());
		model->setEncoding(EncodingType(encoding_cmb->currentText()));

		if(lccollate_cmb->currentIndex() > 0)
			model->setLocalization(LC_COLLATE, lccollate_cmb->currentText());
		else
			model->setLocalization(LC_COLLATE, "");

		if(lcctype_cmb->currentIndex() > 0)
			model->setLocalization(LC_CTYPE, lcctype_cmb->currentText());
		else
			model->setLocalization(LC_CTYPE, "");

		finishConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
