import { Zap, Plug, Car, BarChart3, LogOut } from 'lucide-react';

interface MainMenuProps {
  user: { name: string; email: string } | null;
  onNavigate: (page: 'charging-station' | 'vehicle' | 'consumption') => void;
  onLogout: () => void;
}

export function MainMenu({ user, onNavigate, onLogout }: MainMenuProps) {
  const menuItems = [
    {
      id: 'charging-station',
      title: 'Sélection de Borne',
      description: 'Choisissez votre borne de recharge',
      icon: Plug,
      color: 'from-blue-500 to-blue-600',
      page: 'charging-station' as const
    },
    {
      id: 'vehicle',
      title: 'Sélection de Véhicule',
      description: 'Gérez vos véhicules électriques',
      icon: Car,
      color: 'from-purple-500 to-purple-600',
      page: 'vehicle' as const
    },
    {
      id: 'consumption',
      title: 'Suivi de Consommation',
      description: 'Consultez vos statistiques de recharge',
      icon: BarChart3,
      color: 'from-green-500 to-green-600',
      page: 'consumption' as const
    }
  ];

  return (
    <div className="min-h-screen bg-gradient-to-br from-gray-50 to-gray-100">
      {/* Header */}
      <header className="bg-white shadow-sm">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4">
          <div className="flex items-center justify-between">
            <div className="flex items-center gap-3">
              <div className="bg-green-600 p-2 rounded-lg">
                <Zap className="w-6 h-6 text-white" />
              </div>
              <div>
                <h1 className="text-xl font-bold text-gray-900">EV Charge Manager</h1>
                <p className="text-sm text-gray-500">Gérez vos recharges facilement</p>
              </div>
            </div>
            <div className="flex items-center gap-4">
              <div className="text-right hidden sm:block">
                <p className="text-sm font-medium text-gray-900">{user?.name}</p>
                <p className="text-xs text-gray-500">{user?.email}</p>
              </div>
              <button
                onClick={onLogout}
                className="flex items-center gap-2 px-4 py-2 text-red-600 hover:bg-red-50 rounded-lg transition-colors"
                title="Déconnexion"
              >
                <LogOut className="w-5 h-5" />
                <span className="hidden sm:inline">Déconnexion</span>
              </button>
            </div>
          </div>
        </div>
      </header>

      {/* Main Content */}
      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-12">
        {/* Welcome Section */}
        <div className="mb-12">
          <h2 className="text-3xl font-bold text-gray-900 mb-2">
            Bienvenue, {user?.name} !
          </h2>
          <p className="text-gray-600">
            Que souhaitez-vous faire aujourd'hui ?
          </p>
        </div>

        {/* Menu Grid */}
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {menuItems.map((item) => {
            const Icon = item.icon;
            return (
              <button
                key={item.id}
                onClick={() => onNavigate(item.page)}
                className="group relative bg-white rounded-2xl shadow-lg hover:shadow-xl transition-all duration-300 overflow-hidden p-6 text-left hover:-translate-y-1"
              >
                {/* Gradient Background */}
                <div className={`absolute top-0 right-0 w-32 h-32 bg-gradient-to-br ${item.color} opacity-10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500`} />
                
                {/* Icon */}
                <div className={`inline-flex p-4 rounded-xl bg-gradient-to-br ${item.color} mb-4 shadow-lg group-hover:scale-110 transition-transform duration-300`}>
                  <Icon className="w-8 h-8 text-white" />
                </div>

                {/* Content */}
                <h3 className="text-xl font-bold text-gray-900 mb-2">
                  {item.title}
                </h3>
                <p className="text-gray-600 text-sm">
                  {item.description}
                </p>

                {/* Arrow indicator */}
                <div className="mt-4 flex items-center text-sm font-medium text-gray-400 group-hover:text-green-600 transition-colors">
                  <span>Accéder</span>
                  <svg className="w-4 h-4 ml-1 group-hover:translate-x-1 transition-transform" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 5l7 7-7 7" />
                  </svg>
                </div>
              </button>
            );
          })}
        </div>

        {/* Quick Stats */}
        <div className="mt-12 grid grid-cols-1 md:grid-cols-3 gap-6">
          <div className="bg-white rounded-xl p-6 shadow">
            <div className="flex items-center justify-between">
              <div>
                <p className="text-sm text-gray-500 mb-1">Sessions ce mois</p>
                <p className="text-2xl font-bold text-gray-900">24</p>
              </div>
              <div className="bg-blue-100 p-3 rounded-lg">
                <Plug className="w-6 h-6 text-blue-600" />
              </div>
            </div>
          </div>

          <div className="bg-white rounded-xl p-6 shadow">
            <div className="flex items-center justify-between">
              <div>
                <p className="text-sm text-gray-500 mb-1">Énergie consommée</p>
                <p className="text-2xl font-bold text-gray-900">847 kWh</p>
              </div>
              <div className="bg-green-100 p-3 rounded-lg">
                <Zap className="w-6 h-6 text-green-600" />
              </div>
            </div>
          </div>

          <div className="bg-white rounded-xl p-6 shadow">
            <div className="flex items-center justify-between">
              <div>
                <p className="text-sm text-gray-500 mb-1">Véhicules actifs</p>
                <p className="text-2xl font-bold text-gray-900">2</p>
              </div>
              <div className="bg-purple-100 p-3 rounded-lg">
                <Car className="w-6 h-6 text-purple-600" />
              </div>
            </div>
          </div>
        </div>
      </main>
    </div>
  );
}
