#include "generalconfigwidget.h"

GeneralConfigWidget::GeneralConfigWidget(QWidget * parent) : QWidget(parent)
{
	QPrinter::PaperSize paper_ids[]={QPrinter::A0, QPrinter::A1, QPrinter::A2, QPrinter::A3, QPrinter::A4, QPrinter::A5,
																	 QPrinter::A6, QPrinter::A7, QPrinter::A8, QPrinter::A9, QPrinter::B0, QPrinter::B1,
																	 QPrinter::B10, QPrinter::B2, QPrinter::B3, QPrinter::B4, QPrinter::B5, QPrinter::B6,
																	 QPrinter::B7, QPrinter::B8, QPrinter::B9, QPrinter::C5E, QPrinter::Comm10E, QPrinter::DLE,
																	 QPrinter::Executive, QPrinter::Folio, QPrinter::Ledger, QPrinter::Legal, QPrinter::Letter,
																	 QPrinter::Tabloid, QPrinter::Custom };

	Ui_GeneralConfigWidget::setupUi(this);

	for(int i=0; i < 31; i++)
		paper_cmb->setItemData(i, QVariant(paper_ids[i]));

	connect(unity_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(convertMarginUnity(void)));
	connect(autosave_interv_chk, SIGNAL(toggled(bool)), autosave_interv_spb, SLOT(setEnabled(bool)));

	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]="";
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]="";
}

void GeneralConfigWidget::loadConfiguration(void)
{
	QStringList margin;
	vector<QString> key_attribs;
	unsigned interv;

	key_attribs.push_back(ParsersAttributes::ID);
	BaseConfigWidget::loadConfiguration(GlobalAttributes::GENERAL_CONF, key_attribs);

	grid_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]).toUInt());
	oplist_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]).toUInt());

	interv=(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]).toUInt();
	autosave_interv_chk->setChecked(interv > 0);
	autosave_interv_spb->setValue(interv);
	autosave_interv_spb->setEnabled(autosave_interv_chk->isChecked());

	save_wgts_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]==ParsersAttributes::_TRUE_);
	save_session_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]==ParsersAttributes::_TRUE_);

	print_grid_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]==ParsersAttributes::_TRUE_);
	print_pg_num_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]==ParsersAttributes::_TRUE_);

	paper_cmb->setCurrentIndex((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]).toUInt());
	portrait_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::PORTRAIT);
	landscape_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::LANDSCAPE);

	margin=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN].split(",");

	left_marg->setValue((margin.count() >= 1 ? margin[0].toFloat() : 10));
	top_marg->setValue((margin.count()>= 2 ? margin[1].toFloat() : 10));
	right_marg->setValue((margin.count() >= 3 ? margin[2].toFloat() : 10));
	bottom_marg->setValue((margin.count() >= 4 ? margin[3].toFloat() : 10));

	this->applyConfiguration();
}

void GeneralConfigWidget::saveConfiguration()
{
	try
	{
		map<QString, map<QString, QString> >::iterator itr, itr_end;
		QString widget_sch, file_sch, root_dir;

		root_dir=GlobalAttributes::CONFIGURATIONS_DIR +
						 GlobalAttributes::DIR_SEPARATOR;

		widget_sch=root_dir +
							 GlobalAttributes::SCHEMAS_DIR +
							 GlobalAttributes::DIR_SEPARATOR +
							 ParsersAttributes::WIDGET +
							 GlobalAttributes::SCHEMA_EXT;

		file_sch=root_dir +
						 GlobalAttributes::SCHEMAS_DIR +
						 GlobalAttributes::DIR_SEPARATOR +
						 ParsersAttributes::_FILE_ +
						 GlobalAttributes::SCHEMA_EXT;

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString("%1").arg(grid_size_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString("%1").arg(oplist_size_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString("%1").arg(autosave_interv_chk->isChecked() ? autosave_interv_spb->value() : 0);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString("%1").arg(paper_cmb->currentIndex());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=(portrait_rb->isChecked() ? ParsersAttributes::PORTRAIT : ParsersAttributes::LANDSCAPE);

		unity_cmb->setCurrentIndex(0);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]=QString("%1,%2,%3,%4").arg(left_marg->value())
																																										 .arg(top_marg->value())
																																										 .arg(right_marg->value())
																																										 .arg(bottom_marg->value());

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]=(autosave_interv_chk->isChecked() ? "1" : "");
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]=(save_wgts_chk->isChecked() ? "1" : "");
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]=(print_pg_num_chk->isChecked() ? "1" : "");
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]=(print_grid_chk->isChecked() ? "1" : "");

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";

		itr=config_params.begin();
		itr_end=config_params.end();

		while(itr!=itr_end)
		{
			if(save_session_chk->isChecked() &&
				 (itr->first).contains(QRegExp(QString("(") +
																			 ParsersAttributes::_FILE_ +
																			 QString(")([0-9]+)"))))
			{
				config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]+=
						SchemaParser::getCodeDefinition(file_sch, itr->second);
			}
			else if(save_wgts_chk->isChecked() &&
							(itr->first).contains(QRegExp(QString("(") +
																						ParsersAttributes::WIDGET +
																						QString(")([0-9]+)"))))
			{
				config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]+=
						SchemaParser::getCodeDefinition(widget_sch, itr->second);
			}

			itr++;
		}

		BaseConfigWidget::saveConfiguration(GlobalAttributes::GENERAL_CONF);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GeneralConfigWidget::applyConfiguration(void)
{
	ObjectsScene::setPageConfiguration(static_cast<QPrinter::PaperSize>(paper_cmb->itemData(paper_cmb->currentIndex()).toInt()),
																		 (portrait_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
																		 QRectF(left_marg->value(), top_marg->value(),
																						right_marg->value(), bottom_marg->value()));

	ObjectsScene::setGridSize(grid_size_spb->value());
	OperationList::setMaximumSize(oplist_size_spb->value());
}

void GeneralConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::GENERAL_CONF);
		this->loadConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GeneralConfigWidget::convertMarginUnity(void)
{
	static int prev_unity=0;
	float conv_factor[]={1.0f, 2.83f, 0.04f, 0.1f},
			left, right, top, bottom;

	left=left_marg->value() / conv_factor[prev_unity];
	right=right_marg->value() / conv_factor[prev_unity];
	bottom=bottom_marg->value() / conv_factor[prev_unity];
	top=top_marg->value() / conv_factor[prev_unity];

	left_marg->setValue(left * conv_factor[unity_cmb->currentIndex()]);
	right_marg->setValue(right * conv_factor[unity_cmb->currentIndex()]);
	bottom_marg->setValue(bottom * conv_factor[unity_cmb->currentIndex()]);
	top_marg->setValue(top * conv_factor[unity_cmb->currentIndex()]);

	prev_unity=unity_cmb->currentIndex();
}

