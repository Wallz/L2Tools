@echo off

REM 
REM ����� �������� ��������� �����. ����������� ���� �� ������ ������ �����.
REM 

REM ���� ��� ���������. ����� ���� ����� .u ����. ������ ��������� � ��������
SET file=skill.usk

REM ������ ������� ������
SET object=at.SkillAction_LocateEffect7

REM ��� ���� ��� ������
SET field=EffectClass

REM �� ��� ��������
SET replace=LineageEffect.su_sleep_ta

java -cp ./l2effect-patcher.jar m0nster.efpatcher.Starter -object %object% -field %field% -replace %replace% -file %file%
pause