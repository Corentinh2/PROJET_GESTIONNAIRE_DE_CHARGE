import { ArrowLeft, TrendingUp, Zap, Battery, Clock, Calendar } from 'lucide-react';
import { BarChart, Bar, LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, PieChart, Pie, Cell } from 'recharts';

interface ConsumptionTrackingProps {
  onBack: () => void;
}

const dailyData = [
  { day: 'Lun', energy: 24 },
  { day: 'Mar', energy: 32 },
  { day: 'Mer', energy: 18 },
  { day: 'Jeu', energy: 28 },
  { day: 'Ven', energy: 35 },
  { day: 'Sam', energy: 42 },
  { day: 'Dim', energy: 38 }
];

const monthlyData = [
  { month: 'Jan', energy: 320 },
  { month: 'Fév', energy: 380 },
  { month: 'Mar', energy: 420 },
  { month: 'Avr', energy: 360 },
  { month: 'Mai', energy: 440 },
  { month: 'Jun', energy: 480 }
];

const vehicleDistribution = [
  { name: 'Tesla Model 3', value: 450, color: '#3b82f6' },
  { name: 'Renault Zoe', value: 280, color: '#8b5cf6' },
  { name: 'VW ID.4', value: 340, color: '#10b981' }
];

const recentSessions = [
  {
    id: '1',
    vehicle: 'Tesla Model 3',
    station: 'Borne A',
    date: '15 Jan 2026',
    time: '14:30',
    duration: '2h 15min',
    energy: '42 kWh',
    cost: '12.60 €'
  },
  {
    id: '2',
    vehicle: 'Renault Zoe',
    date: '14 Jan 2026',
    station: 'Borne C',
    time: '09:15',
    duration: '1h 45min',
    energy: '28 kWh',
    cost: '8.40 €'
  },
  {
    id: '3',
    vehicle: 'VW ID.4',
    station: 'Borne F',
    date: '13 Jan 2026',
    time: '16:00',
    duration: '2h 30min',
    energy: '38 kWh',
    cost: '11.40 €'
  }
];

export function ConsumptionTracking({ onBack }: ConsumptionTrackingProps) {
  return (
    <div className="min-h-screen bg-gray-50">
      {/* Header */}
      <header className="bg-white shadow-sm sticky top-0 z-10">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4">
          <div className="flex items-center gap-4">
            <button
              onClick={onBack}
              className="p-2 hover:bg-gray-100 rounded-lg transition-colors"
            >
              <ArrowLeft className="w-6 h-6 text-gray-700" />
            </button>
            <div>
              <h1 className="text-2xl font-bold text-gray-900">Suivi de Consommation</h1>
              <p className="text-sm text-gray-500">Analysez vos statistiques de recharge</p>
            </div>
          </div>
        </div>
      </header>

      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        {/* Key Metrics */}
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4 mb-8">
          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-2">
              <div className="bg-green-100 p-2 rounded-lg">
                <Zap className="w-5 h-5 text-green-600" />
              </div>
              <TrendingUp className="w-5 h-5 text-green-600" />
            </div>
            <p className="text-2xl font-bold text-gray-900">847 kWh</p>
            <p className="text-sm text-gray-500">Énergie totale</p>
            <p className="text-xs text-green-600 mt-1">+12% ce mois</p>
          </div>

          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-2">
              <div className="bg-blue-100 p-2 rounded-lg">
                <Battery className="w-5 h-5 text-blue-600" />
              </div>
              <TrendingUp className="w-5 h-5 text-blue-600" />
            </div>
            <p className="text-2xl font-bold text-gray-900">24</p>
            <p className="text-sm text-gray-500">Sessions totales</p>
            <p className="text-xs text-blue-600 mt-1">+3 cette semaine</p>
          </div>

          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-2">
              <div className="bg-purple-100 p-2 rounded-lg">
                <Clock className="w-5 h-5 text-purple-600" />
              </div>
            </div>
            <p className="text-2xl font-bold text-gray-900">42h 15m</p>
            <p className="text-sm text-gray-500">Temps de charge</p>
            <p className="text-xs text-gray-400 mt-1">Moyenne: 1h 45m</p>
          </div>

          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-2">
              <div className="bg-yellow-100 p-2 rounded-lg">
                <span className="text-xl">€</span>
              </div>
            </div>
            <p className="text-2xl font-bold text-gray-900">254.10 €</p>
            <p className="text-sm text-gray-500">Coût total</p>
            <p className="text-xs text-gray-400 mt-1">0.30 €/kWh</p>
          </div>
        </div>

        {/* Charts Row */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6 mb-8">
          {/* Daily Consumption */}
          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-6">
              <h3 className="text-lg font-bold text-gray-900">Consommation hebdomadaire</h3>
              <Calendar className="w-5 h-5 text-gray-400" />
            </div>
            <ResponsiveContainer width="100%" height={250}>
              <BarChart data={dailyData}>
                <CartesianGrid strokeDasharray="3 3" stroke="#e5e7eb" />
                <XAxis dataKey="day" stroke="#6b7280" />
                <YAxis stroke="#6b7280" />
                <Tooltip 
                  contentStyle={{ backgroundColor: '#fff', border: '1px solid #e5e7eb', borderRadius: '8px' }}
                  formatter={(value) => [`${value} kWh`, 'Énergie']}
                />
                <Bar dataKey="energy" fill="#10b981" radius={[8, 8, 0, 0]} />
              </BarChart>
            </ResponsiveContainer>
          </div>

          {/* Monthly Trend */}
          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <div className="flex items-center justify-between mb-6">
              <h3 className="text-lg font-bold text-gray-900">Tendance mensuelle</h3>
              <TrendingUp className="w-5 h-5 text-green-600" />
            </div>
            <ResponsiveContainer width="100%" height={250}>
              <LineChart data={monthlyData}>
                <CartesianGrid strokeDasharray="3 3" stroke="#e5e7eb" />
                <XAxis dataKey="month" stroke="#6b7280" />
                <YAxis stroke="#6b7280" />
                <Tooltip 
                  contentStyle={{ backgroundColor: '#fff', border: '1px solid #e5e7eb', borderRadius: '8px' }}
                  formatter={(value) => [`${value} kWh`, 'Énergie']}
                />
                <Line 
                  type="monotone" 
                  dataKey="energy" 
                  stroke="#3b82f6" 
                  strokeWidth={3}
                  dot={{ fill: '#3b82f6', r: 5 }}
                />
              </LineChart>
            </ResponsiveContainer>
          </div>
        </div>

        {/* Distribution and Recent Sessions */}
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* Vehicle Distribution */}
          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200">
            <h3 className="text-lg font-bold text-gray-900 mb-6">Consommation par véhicule</h3>
            <ResponsiveContainer width="100%" height={200}>
              <PieChart>
                <Pie
                  data={vehicleDistribution}
                  cx="50%"
                  cy="50%"
                  innerRadius={50}
                  outerRadius={80}
                  paddingAngle={5}
                  dataKey="value"
                >
                  {vehicleDistribution.map((entry, index) => (
                    <Cell key={`cell-${index}`} fill={entry.color} />
                  ))}
                </Pie>
                <Tooltip formatter={(value) => `${value} kWh`} />
              </PieChart>
            </ResponsiveContainer>
            <div className="space-y-2 mt-4">
              {vehicleDistribution.map((item) => (
                <div key={item.name} className="flex items-center justify-between text-sm">
                  <div className="flex items-center gap-2">
                    <div className="w-3 h-3 rounded-full" style={{ backgroundColor: item.color }} />
                    <span className="text-gray-700">{item.name}</span>
                  </div>
                  <span className="font-medium text-gray-900">{item.value} kWh</span>
                </div>
              ))}
            </div>
          </div>

          {/* Recent Sessions */}
          <div className="bg-white rounded-xl p-6 shadow-sm border border-gray-200 lg:col-span-2">
            <h3 className="text-lg font-bold text-gray-900 mb-4">Sessions récentes</h3>
            <div className="space-y-3">
              {recentSessions.map((session) => (
                <div
                  key={session.id}
                  className="border border-gray-200 rounded-lg p-4 hover:bg-gray-50 transition-colors"
                >
                  <div className="flex items-start justify-between mb-2">
                    <div>
                      <h4 className="font-medium text-gray-900">{session.vehicle}</h4>
                      <p className="text-sm text-gray-500">{session.station}</p>
                    </div>
                    <span className="text-sm font-bold text-green-600">{session.energy}</span>
                  </div>
                  <div className="flex items-center justify-between text-xs text-gray-500">
                    <div className="flex items-center gap-4">
                      <span className="flex items-center gap-1">
                        <Calendar className="w-3 h-3" />
                        {session.date}
                      </span>
                      <span className="flex items-center gap-1">
                        <Clock className="w-3 h-3" />
                        {session.time}
                      </span>
                    </div>
                    <div className="flex items-center gap-3">
                      <span>{session.duration}</span>
                      <span className="font-medium text-gray-900">{session.cost}</span>
                    </div>
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>
      </main>
    </div>
  );
}
