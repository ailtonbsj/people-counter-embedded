unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, ComCtrls, StdCtrls, ExtCtrls, WinSkinData, CPort, Shellapi;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    Arquivo1: TMenuItem;
    Conectar1: TMenuItem;
    Salvarinformaes1: TMenuItem;
    Fechar1: TMenuItem;
    Editar1: TMenuItem;
    Configuraes1: TMenuItem;
    Aalterarfonte1: TMenuItem;
    Desconetar1: TMenuItem;
    Ajuda1: TMenuItem;
    Acessarsite1: TMenuItem;
    Sobre1: TMenuItem;
    Panel1: TPanel;
    Button1: TButton;
    Memo1: TMemo;
    StatusBar1: TStatusBar;
    ComPort1: TComPort;
    SkinData1: TSkinData;
    Button2: TButton;
    FontDialog1: TFontDialog;
    SaveDialog1: TSaveDialog;
    procedure Fechar1Click(Sender: TObject);
    procedure Sobre1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Configuraes1Click(Sender: TObject);
    procedure Aalterarfonte1Click(Sender: TObject);
    procedure FontDialog1Apply(Sender: TObject; Wnd: HWND);
    procedure Acessarsite1Click(Sender: TObject);
    procedure Salvarinformaes1Click(Sender: TObject);
    procedure Conectar1Click(Sender: TObject);
    procedure Desconetar1Click(Sender: TObject);
    procedure ComPort1AfterClose(Sender: TObject);
    procedure ComPort1AfterOpen(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure ComPort1RxChar(Sender: TObject; Count: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Fechar1Click(Sender: TObject);
begin
close;
end;

procedure TForm1.Sobre1Click(Sender: TObject);
begin
ShowMessage('DESENVOLVIDO POR:'+#13+'José Ailton B. S.'+#13+'E-MAIL:'+#13+'ailton-linux@hotmail.com'+#13+'SITE:'+#13+'www.novalogica.wordpress.com');
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
Memo1.Lines.Clear;
end;

procedure TForm1.Configuraes1Click(Sender: TObject);
begin
ComPort1.ShowSetupDialog;
end;

procedure TForm1.Aalterarfonte1Click(Sender: TObject);
begin
FontDialog1.Execute;
end;

procedure TForm1.FontDialog1Apply(Sender: TObject; Wnd: HWND);
begin
Memo1.Font:=FontDialog1.Font;
end;

procedure TForm1.Acessarsite1Click(Sender: TObject);
begin
ShellExecute(0,'open','http://www.novalogica.wordpress.com',nil,nil,SW_SHOW);
end;

procedure TForm1.Salvarinformaes1Click(Sender: TObject);
begin
if SaveDialog1.Execute then
Memo1.Lines.SaveToFile(SaveDialog1.FileName);
end;

procedure TForm1.Conectar1Click(Sender: TObject);
begin
ComPort1.Open;
end;

procedure TForm1.Desconetar1Click(Sender: TObject);
begin
ComPort1.Close;
end;

procedure TForm1.ComPort1AfterClose(Sender: TObject);
begin
StatusBar1.Panels[0].Text:='DESCONECTADO!!!';
Button1.Caption:='CONECTAR';
end;

procedure TForm1.ComPort1AfterOpen(Sender: TObject);
begin
StatusBar1.Panels[0].Text:='CONECTADO!!!';
Button1.Caption:='DESCONECTAR';
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
if StatusBar1.Panels[0].Text='CONECTADO!!!' THEN
ComPort1.Close
ELSE
ComPort1.Open;
end;

procedure TForm1.ComPort1RxChar(Sender: TObject; Count: Integer);
var buffer: AnsiString;
 contador: integer;
begin
{caso receber dados da serial este procedimento eh executado}
sleep(500); // espere 500 ms
contador:=ComPort1.InputCount; // capture tamanho do dado
ComPort1.ReadStr(buffer,contador); //guarde em Buffer
{a linha seguinte adiciona buffer ao Memo1 concatenado com data e hora}
Memo1.Lines.Add('--------------------------');
Memo1.Lines.Add('Quantidade Atual é: '+ buffer);
Memo1.Lines.Add('Dia: '+ DateToStr(Date));
Memo1.Lines.Add('Hora: '+ TimeToStr(Time));
end;

end.
